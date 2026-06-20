from django.shortcuts import render
import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth.hashers import make_password, check_password
from django.core.exceptions import ObjectDoesNotExist
from django.core import signing

from apps.accounts.models import User
from apps.core.models import Notification
from channels.layers import get_channel_layer
from asgiref.sync import async_to_sync


@csrf_exempt
def create_notification(text, user, status, event_id=None):
    new_notification = Notification(receiving=user,
                                    text=text,
                                    status=status,
                                    event_id=event_id)
    new_notification.save()
    channel_layer = get_channel_layer()
    async_to_sync(channel_layer.group_send)(f'user_{user.id}', {
        'type': 'notify_user',
    })


@csrf_exempt
def get_notifications(request):
    if request.method != 'GET':
        return JsonResponse({'error': 'method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'User not authenticated'}, status=400)
    try:
        user = User.objects.get(id=user_id)
    except:
        return JsonResponse({'error': 'User not found'}, status=403)

    notifications = Notification.objects.filter(receiving=user).values(
        'id', 'text', 'status', 'created_at', 'event_id')
    return JsonResponse({
        'success': True,
        'notifications': list(notifications)
    },
                        status=200)


@csrf_exempt
def delete_notification(request):
    if request.method != 'DELETE':
        return JsonResponse({'error': 'method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'User not authenticated'}, status=400)
    try:
        user = User.objects.get(id=user_id)
    except:
        return JsonResponse({'error': 'User not found'}, status=403)

    try:
        data = json.loads(request.body)
        notification_id = data.get('notification_id')
        if not notification_id:
            return JsonResponse({'error': 'Missing notification_id'}, status=400)
    except:
        return JsonResponse({'error': 'Invalid JSON'}, status=400)

    try:
        notification = Notification.objects.get(id=notification_id, receiving=user)
        notification.delete()
        return JsonResponse({'success': True, 'message': 'Notification deleted'}, status=200)
    except Notification.DoesNotExist:
        return JsonResponse({'error': 'Notification not found or access denied'}, status=404)
