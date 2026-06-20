from django.shortcuts import render, redirect
import os
import requests
import json
import re
import os
import requests
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.hashers import make_password, check_password
from django.core.exceptions import ObjectDoesNotExist, ValidationError
from django.db import IntegrityError
from django.core import signing
from django.core.validators import validate_email
from django.contrib.auth.password_validation import validate_password

from apps.accounts.models import User
from apps.core.models import Statistic


def build_avatar_payload(user, request=None):
    profile_picture = None
    if user.profile_picture:
        if request is not None:
            profile_picture = request.build_absolute_uri(user.profile_picture.url)
        else:
            profile_picture = user.profile_picture.url
    return {
        "emoji": user.emoji,
        "profile_picture": profile_picture,
    }


# Create your views here.

#login, logout e registrazione utenti


@csrf_exempt
def register(request):
    if request.method != "POST":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    try:
        data = json.loads(request.body)
    except json.JSONDecodeError:
        return JsonResponse({"error": "Invalid JSON"}, status=400)

    username = data.get("username", "").strip()
    email = data.get("email", "").strip()
    raw_password = data.get("password", "").strip()

    if not username or not email or not raw_password:
        return JsonResponse({"error": "All fields required"}, status=400)

    if not re.match(r'^[a-zA-Z0-9_]{3,30}$', username):
        return JsonResponse(
            {
                'error':
                'Username must be 3-30 characters, only letters, numbers, and underscore'
            },
            status=400)

    try:
        validate_email(email)
    except ValidationError:
        return JsonResponse({"error": "Email is not valid"}, status=400)

    try:
        validate_password(raw_password, user=None)
    except ValidationError as e:
        return JsonResponse(
            {'error': 'Password not strong enough: ' + ', '.join(e.messages)},
            status=400)

    if User.objects.filter(username=username).exists():
        return JsonResponse({"error": "Username already taken"}, status=400)
    if User.objects.filter(email=email).exists():
        return JsonResponse({"error": "Email already used"}, status=400)

    hashed = make_password(raw_password)
    new_user = User(username=username, email=email, password=hashed, emoji="👤")
    new_user.save()
    statistic = Statistic(user=new_user)
    statistic.save()
    return JsonResponse({
        'success': True,
        'message': 'User created'
    },
                        status=201)


@csrf_exempt
def login(request):
    if request.method != "POST":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    try:
        data = json.loads(request.body)
    except json.JSONDecodeError:
        return JsonResponse({"error": "Invalid JSON"}, status=400)

    username = data.get("username")
    raw_password = data.get("password")

    if not username or not raw_password:
        return JsonResponse({'success': False, 'message': 'Missing fields'}, status=400)

    try:
        current_user = User.objects.get(username=username)
    except ObjectDoesNotExist:
        try:
            current_user = User.objects.get(email=username)
        except:
            return JsonResponse({
                'success': False,
                'message': 'User not found'
            },
                                status=404)

    if not current_user.password:
        return JsonResponse({
            'success': False,
            'message': 'Please login using 42 authentication for this username'
        }, status=400)


    if check_password(raw_password, current_user.password):
        # Set session
        request.session["user_id"] = current_user.id
        request.session["username"] = current_user.username
        request.session["emoji"] = current_user.emoji
        request.session["profile_picture"] = build_avatar_payload(
            current_user, request
        )["profile_picture"]

        avatar_payload = build_avatar_payload(current_user, request)

        response = JsonResponse(
            {
                "success": True,
                "message": "Login done",
                "user_id": current_user.id,
                "username": current_user.username,
                **avatar_payload,
            },
            status=200,
        )
        return response
    else:
        return JsonResponse({"success": False, "message": "Wrong password"}, status=401)


@csrf_exempt
def logout(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    request.session.flush()

    response = JsonResponse(
        {
            'success': True,
            'message': 'Logged out successfully'
        }, status=200)

    response.delete_cookie('sessionid')
    return response


@csrf_exempt
def get_session(request):
    if request.method != "GET":
        return JsonResponse({"error": "Methos not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"authenticated": False}, status=401)

    try:
        current_user = User.objects.get(id=user_id)
    except ObjectDoesNotExist:
        return JsonResponse({'error': 'User not found'}, status=404)

    return JsonResponse(
        {
            "success": True,
            "user_id": current_user.id,
            "username": current_user.username,
            **build_avatar_payload(current_user, request),
        },
        status=200,
    )


def oauth_callback(request):
    code = request.GET.get("code")
    if not code:
        return JsonResponse({"error": "Missing code"}, status=400)

    client_id = os.environ.get("FT_CLIENT_ID")
    client_secret = os.environ.get("FT_CLIENT_SECRET")
    frontend_base = os.environ.get("FRONTEND_URL", "http://localhost:3000")
    redirect_uri = os.environ.get("OAUTH_REDIRECT_URI") or request.build_absolute_uri(
        request.path
    )

    if not client_id or not client_secret:
        return JsonResponse(
            {"error": "OAuth client credentials not configured"}, status=500
        )

    token_url = "https://api.intra.42.fr/oauth/token"
    token_resp = None
    try:
        token_resp = requests.post(
            token_url,
            data={
                "grant_type": "authorization_code",
                "client_id": client_id,
                "client_secret": client_secret,
                "code": code,
                "redirect_uri": redirect_uri,
            },
            timeout=10,
        )
        token_resp.raise_for_status()
        access_token = token_resp.json().get("access_token")
    except Exception as e:
        details = str(e)
        if token_resp is not None:
            try:
                details = f"{token_resp.status_code} {token_resp.text}"
            except Exception:
                details = str(e)
        return JsonResponse(
            {"error": "Token exchange failed", "details": details},
            status=500,
        )

    try:
        profile_resp = requests.get(
            "https://api.intra.42.fr/v2/me",
            headers={"Authorization": f"Bearer {access_token}"},
            timeout=10,
        )
        profile_resp.raise_for_status()
        profile = profile_resp.json()
    except Exception as e:
        return JsonResponse(
            {"error": "Failed to fetch profile", "details": str(e)}, status=500
        )

    intra_id = str(profile.get("id"))
    email = profile.get("email") or ""
    login_name = profile.get("login") or (
        email.split("@")[0] if email else f"user{intra_id}"
    )

    try:
        user = User.objects.filter(intra_id=intra_id).first()
        if not user and email:
            user = User.objects.filter(email=email).first()
            if user:
                user.intra_id = intra_id
                user.save()

        if not user:
            base = login_name
            username = base
            i = 0
            while User.objects.filter(username=username).exists():
                i += 1
                username = f"{base}{i}"
            new_user = User(
                username=username,
                email=email or None,
                password=None,
                emoji="👽",
                intra_id=intra_id,
            )
            new_user.save()
            statistic = Statistic(user=new_user)
            statistic.save()
            user = new_user

        request.session["user_id"] = user.id
        request.session["username"] = user.username
        request.session["emoji"] = user.emoji
        request.session["profile_picture"] = build_avatar_payload(user, request)[
            "profile_picture"
        ]

        return redirect(frontend_base + "/play")
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=500)
