from django.shortcuts import render
import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.hashers import make_password, check_password
from django.core.exceptions import ObjectDoesNotExist
from django.core import signing
from django.db.models import Q
from django.db.models import F

from apps.accounts.models import Friends, User
from apps.core.models import Notification
from ...core.views.notifications import create_notification
from ...accounts.utils.general_badges import social_butterfly


@csrf_exempt
def getFriends(request):
    if request.method != "GET":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"authenticated": False}, status=401)

    try:
        current_user = User.objects.get(id=user_id)
    except ObjectDoesNotExist:
        return JsonResponse({"error": "User not found"}, status=404)

    sent_pending = Friends.objects.filter(user1=current_user,status="pending").values(
        "id",
        user_id=F("user2__id"),
        username=F("user2__username"),
        online=F("user2__is_online"),
        emoji= F("user2__emoji")
    )

    sent_accepted = Friends.objects.filter(
        user1=current_user, status="accepted"
    ).values(
        "id",
        user_id = F("user2__id"), 
        username =F("user2__username"),
        online=F("user2__is_online"), 
        emoji=F("user2__emoji"))

    received_pending = Friends.objects.filter(user2=current_user,status="pending"
                        ).values(
                        "id",
                        user_id=F("user1__id"),
                        username=F("user1__username"),
                        online=F("user1__is_online"),
                        emoji=F("user1__emoji"),
                        )

    received_accepted = Friends.objects.filter(
                        user2=current_user, status="accepted"
                        ).values(
                        "id",
                        user_id=F("user1__id"), 
                        username=F("user1__username"), 
                        online=F("user1__is_online"), 
                        emoji=F("user1__emoji"))

    return JsonResponse(
        {
            "user_id": current_user.id,
            "username": current_user.username,
            "friends": {
                "sent_pending": list(sent_pending),
                "accepted": list(sent_accepted) + list(received_accepted),
                "received_pending": list(received_pending),
            },
        },
        status=200,
    )


@csrf_exempt
def get_users(request):
    if request.method != "GET":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    search_query = request.GET.get("search", "").strip()

    if not search_query:
        return JsonResponse({"error": "Search query required"}, status=400)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"authenticated": False}, status=401)

    try:
        current_user = User.objects.get(id=user_id)
    except ObjectDoesNotExist:
        return JsonResponse({"error": "User not found"}, status=404)

    users = (
        User.objects.filter(username__istartswith=search_query)
        .exclude(id=current_user.id)
        .values("id", "username", "email", "is_online", "created_at")[:5]
    )

    return JsonResponse(
        {"search_query": search_query, "count": len(users), "users": list(users)},
        status=200,
    )


@csrf_exempt
def send_request(request):
    if request.method != "POST":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"authenticated": False}, status=401)

    try:
        current_user = User.objects.get(id=user_id)
    except ObjectDoesNotExist:
        return JsonResponse({"error": "User not found"}, status=404)

    try:
        data = json.loads(request.body)
    except:
        return JsonResponse({"error": "Invalid Json"}, status=400)
    user2_id = data.get("user2", "")
    if not user2_id:
        return JsonResponse({"error": "Missing field"}, status=201)

    try:
        user = User.objects.get(id=user2_id)
    except:
        return JsonResponse({"error": "User not found"}, status=404)

    if user.id == current_user.id:
        return JsonResponse({"error": "Cannot add yourself as friend"}, status=400)

    existing = Friends.objects.filter(
        (Q(user1=current_user, user2=user) | Q(user1=user, user2=current_user))
    ).exists()

    if existing:
        return JsonResponse({"error": "Friend request already exists"}, status=400)

    newRequest = Friends(user1=current_user, user2=user, status="pending")
    newRequest.save()
    emoji = current_user.emoji if current_user.emoji else "👤"
    text = f"{emoji} {current_user.username}"
    create_notification(text, user, "pending", newRequest.id)
    return JsonResponse(
        {"success": True, "message": "request sent", "request_id": newRequest.id},
        status=200,
    )


@csrf_exempt
def accept_request(request):
    if request.method != "POST":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"error": "User not authenticated"}, status=400)
    try:
        current_user = User.objects.get(id=user_id)
    except:
        return JsonResponse({"error", "User not found"}, status=404)

    try:
        data = json.loads(request.body)
        request_id = data.get("request_id")
        try:
            current_request = Friends.objects.get(id=request_id)
            notification = Notification.objects.filter(event_id=str(request_id))
        except:
            return JsonResponse({"error": "Request not found"}, status=404)

        if current_request.status == "pending":
            current_request.status = "accepted"
            current_request.save()
            notification.delete()
            
            emoji = current_user.emoji if current_user.emoji else "👤"
            text = f"{emoji} {current_user.username}"
            create_notification(text, current_request.user1, "accepted")
            try:
                social_butterfly(current_user)
                social_butterfly(current_request.user1)
            except:
                print("failed")
            return JsonResponse(
                {
                    "success": True,
                    "message": "request modified",
                    "request_id": current_request.id,
                },
                status=200,
            )
        else:
            return JsonResponse({"error": "Request already accepted"}, status=408)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=500)


@csrf_exempt
def remove_friend(request):
    if request.method != "DELETE":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"error": "Not logged in"}, status=400)
    try:
        current_user = User.objects.get(id=user_id)
    except:
        return JsonResponse({"error": "User not found"}, status=404)

    try:
        data = json.loads(request.body)
        request_id = data.get("request_id")
        if not request_id:
            return JsonResponse({"error": "Missing field"}, status=401)
    except:
        return JsonResponse({"error": "Invalid Json"}, status=402)

    try:
        to_delete = Friends.objects.get(id=request_id)
        if (
            to_delete.user1.id != current_user.id
            and to_delete.user2.id != current_user.id
        ):
            return JsonResponse(
                {"error": "You cannot delete this friendship"}, status=403
            )

        to_delete.delete()
        Notification.objects.filter(event_id=str(request_id)).delete()
        return JsonResponse({"success": True, "message": "Friend removed"}, status=200)
    except Friends.DoesNotExist:
        return JsonResponse({"error": "Request not found"}, status=408)
    except Exception as e:
        return JsonResponse({"error": str(e)}, status=500)

@csrf_exempt
def invite_player(request):
    if request.method != "POST":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    user_id = request.session.get("user_id")
    if not user_id:
        return JsonResponse({"error": "Not logged in"}, status=401)

    try:
        current_user = User.objects.get(id=user_id)
    except User.DoesNotExist:
        return JsonResponse({"error": "User not found"}, status=404)

    try:
        data = json.loads(request.body)
        friend_username = data.get("friend_name")
        room_code = data.get("roomcode")

        if not friend_username or not room_code:
            return JsonResponse({"error": "Missing field"}, status=400)
    except json.JSONDecodeError:
        return JsonResponse({"error": "Invalid JSON"}, status=400)

    try:
        friend = User.objects.get(username=friend_username)
    except User.DoesNotExist:
        return JsonResponse({"error": "Friend not found"}, status=404)

    is_friend = Friends.objects.filter(
        (Q(user1=current_user, user2=friend) | Q(user1=friend, user2=current_user)),
        status="accepted"
    ).exists()

    if not is_friend:
        return JsonResponse({"error": "You are not friends with this user"}, status=403)
    
    text = f"{current_user.username}"

    create_notification(text, friend, "invite_to_game", room_code)

    return JsonResponse(
        {"success": True, "message": "request sent"},
        status=200,
    )