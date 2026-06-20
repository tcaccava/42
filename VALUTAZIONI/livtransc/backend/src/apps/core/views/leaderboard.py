from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt

from apps.core.models import Statistic

def leaderboard_leader():
    xp = Statistic.objects.all().order_by('-xp').values(
        'user__username', 'user__emoji', 'xp')[:10]
    if not xp:
        return None
    return xp[0].get('user__username')

@csrf_exempt
def get_leaderboard(request):
    if (request.method != 'GET'):
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    victories = Statistic.objects.all().order_by('-victories').values(
        'user__username', 'user__emoji', 'victories')
    xp = Statistic.objects.all().order_by('-xp').values(
        'user__username', 'user__emoji', 'xp')
    streak = Statistic.objects.all().order_by('-highest_streak').values(
        'user__username', 'user__emoji', 'highest_streak')
    return JsonResponse(
        {
            'success': True,
            'victories': list(victories),
            'streak': list(streak),
            'xp': list(xp)
        },
        status=200)
