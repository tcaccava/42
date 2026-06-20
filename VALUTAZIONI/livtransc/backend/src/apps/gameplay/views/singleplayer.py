from django.shortcuts import render
import json
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.exceptions import ObjectDoesNotExist
from django.db.models import Q

from apps.accounts.models import User
from apps.core.models import Statistic
from apps.gameplay.models import OldMatches, Question, SinglePlayerMatch
import random
from django.utils import timezone
from ...accounts.utils.match_badges import check_match_badges
from ...accounts.utils.question_badges import check_question_badges, still_undecided
from ...accounts.utils.general_badges import lightning_bolt
from ..utils.game_utils import add_xp
from .localMultiplayer import questions


@csrf_exempt
def update_old_match(current_match, status):
    old_match = OldMatches(user=current_match.user,
                           correct_answers=current_match.question_number - 1,
                           mode='play.singleplayer')
    old_match.save()
    if status == 0:
        old_match.status = 0
    elif status == -1:
        old_match.status = -1
    old_match.save()


@csrf_exempt
def victory(statistics, current_match):
    check_question_badges(current_match, statistics.user, 1)
    statistics.correct_answers += 1
    statistics.sp_correct_answers += 1
    statistics.wins_in_a_row += 1
    statistics.victories += 1
    statistics.consecutive_correct_answers += 1
    if statistics.consecutive_correct_answers > statistics.highest_streak:
        statistics.highest_streak = statistics.consecutive_correct_answers
    add_xp(50, statistics)
    update_old_match(current_match, 0)
    check_match_badges(current_match.user)
    current_match.delete()


@csrf_exempt
def defeat(statistics, current_match, flag):
    check_question_badges(current_match, statistics.user, 0)
    update_old_match(current_match, -1)

    if statistics.consecutive_correct_answers > statistics.highest_streak:
        statistics.highest_streak = statistics.consecutive_correct_answers
    statistics.consecutive_correct_answers = 0
    statistics.wins_in_a_row = 0
    statistics.lost = statistics.lost + 1
    if flag == 1:
        statistics.wrong_answers = statistics.wrong_answers + 1
    statistics.save()
    check_match_badges(current_match.user)
    current_match.delete()


@csrf_exempt
def create_match(user, mode):
    SinglePlayerMatch.objects.filter(user=user).delete()

    if mode == 'SP':
        question_ids = questions(16)
        new_match = SinglePlayerMatch(user=user, question_ids=question_ids)

    new_match.save()
    new_match.last_interaction = new_match.started_at
    new_match.save()

    statistics, _ = Statistic.objects.get_or_create(user=user)
    statistics.games += 1
    if mode == 'SP':
        statistics.single_player_games += 1

    statistics.save()
    return (new_match)


@csrf_exempt
def start_match(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Invalid Method'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'User not authenticated'}, status=400)
    try:
        current_user = User.objects.get(id=user_id)
    except:
        return JsonResponse({'error': 'user not found'}, status=404)

    new_match = create_match(current_user, 'SP')
    question = Question.objects.get(id=new_match.question_ids[0])
    return JsonResponse(
        {
            'success': True,
            'match_id': new_match.id,
            'question': {
                'number': new_match.question_number,
                'text': question.text,
                'options': [question.a, question.b, question.c, question.d],
            },
        },
        status=200)


@csrf_exempt
def update_match(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'User not authenticated'}, status=401)

    try:
        data = json.loads(request.body)
        match_id = data.get('match_id')
        user_answer = data.get('user_answer')
        question_index = data.get('question_index')
        double_chance = data.get('double_chance')

        if not match_id or not user_answer or question_index is None:
            return JsonResponse({'error': 'Missing fields'}, status=400)
    except:
        return JsonResponse({'error': 'Invalid JSON'}, status=400)

    try:
        current_match = SinglePlayerMatch.objects.get(id=match_id,
                                                      user_id=user_id)
        statistics, _ = Statistic.objects.get_or_create(user_id=user_id)
    except SinglePlayerMatch.DoesNotExist:
        return JsonResponse({'error': 'Match not found'}, status=404)

    now = timezone.now()
    elapsed_time = now - current_match.last_interaction
    seconds_passed = elapsed_time.total_seconds()
    if seconds_passed > 70:
        defeat(statistics, current_match, 0)
        return JsonResponse({'error': 'Time over, match ended'}, status=408)

    try:
        question_id = current_match.question_ids[question_index]
        question = Question.objects.get(id=question_id)
    except (IndexError, Question.DoesNotExist):
        return JsonResponse({'error': 'Question not found'}, status=404)

    is_correct = (user_answer.lower() == question.answer.lower())
    if not is_correct and double_chance and current_match.double_chance:
        statistics.lifeline_used += 1
        statistics.save()
        current_match.double_chance = False
        current_match.save()
        return JsonResponse(
            {
                'success': True,
                'is_correct': False,
                'double_chance_used': True
            },
            status=200)
    elif not is_correct:
        defeat(statistics, current_match, 1)
        return JsonResponse(
            {
                'success': True,
                'is_correct': False,
                'correct_answer': question.answer,
                'error': 'Wrong answer'
            },
            status=200)
    if double_chance:
        statistics.lifeline_used += 1
        statistics.save()
        current_match.double_chance = False
        current_match.save()
    lightning_bolt(statistics, seconds_passed)
    current_match.last_interaction = now
    current_match.question_number += 1
    current_match.save()
    statistics.correct_answers += 1
    statistics.sp_correct_answers += 1
    statistics.consecutive_correct_answers += 1
    add_xp(2 * current_match.question_number, statistics)

    if question_index + 1 >= 15:
        victory(statistics, current_match)
        return JsonResponse(
            {
                'success': True,
                'isCompleted': True,
                'is_correct': True,
                'correct_answer': question.answer
            },
            status=200)

    question = Question.objects.get(
        id=current_match.question_ids[current_match.question_number - 1])
    return JsonResponse(
        {
            'success': True,
            'message': 'Match started',
            'is_correct': True,
            'correct_answer': user_answer,
            'match_id': current_match.id,
            'isCompleted': False,
            'next_question': {
                'number': current_match.question_number,
                'text': question.text,
                'options': [question.a, question.b, question.c, question.d],
            },
        },
        status=200)


@csrf_exempt
def check_session(request):
    if request.method != 'GET':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'User not authenticated'}, status=400)
    try:
        current_user = User.objects.get(id=user_id)
    except:
        return JsonResponse({'error': 'User not found'}, status=404)

    try:
        match = SinglePlayerMatch.objects.get(user=current_user)
        statistics = Statistic.objects.get(user=current_user)
    except:
        return JsonResponse({'success': True, 'existing': False}, status=200)

    now = timezone.now()
    elapsed_time = (now - match.last_interaction).total_seconds()
    if elapsed_time < 70:
        question = Question.objects.get(
            id=match.question_ids[match.question_number - 1])
        return JsonResponse(
            {
                'success': True,
                'existing': True,
                'question_number': match.question_number,
                'match_id': match.id,
                'remaining_time': 64 - elapsed_time,
                'isCompleted': False,
                'next_question': {
                    'number': match.question_number,
                    'text': question.text,
                    'options':
                    [question.a, question.b, question.c, question.d],
                },
                'help': {
                    'fifty_fifty': match.fifty_fifty,
                    'double_chance': match.double_chance,
                    'refill_time': match.refill_time,
                    'change_question': match.change_question
                }
            },
            status=200)

    defeat(statistics, match, 0)
    return JsonResponse({'success': True, 'existing': False}, status=200)


@csrf_exempt
def use_help(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    user_id = request.session.get('user_id')
    if not user_id:
        return JsonResponse({'error': 'user not authenticated'}, status=400)
    try:
        current_user = User.objects.get(id=user_id)
        statistics = Statistic.objects.get(user=current_user)
    except:
        return JsonResponse({'error': 'user not found'}, status=404)

    try:
        match = SinglePlayerMatch.objects.get(user=current_user)
    except:
        return JsonResponse({'error': 'Match not found'}, status=404)

    try:
        data = json.loads(request.body)
        fifty = data.get('fifty')
        change_question = data.get('change_question')
        refill_time = data.get('refill_time')
        elapsed_time = (timezone.now() -
                        match.last_interaction).total_seconds()
        if elapsed_time >= 70:
            return JsonResponse({'error': 'Time already over'}, status=400)

        statistics.lifeline_used += 1
        statistics.save()
        if fifty:
            if not match.fifty_fifty:
                return JsonResponse({'error': 'Lifeline already used'},
                                    status=400)
            still_undecided(match, match.question_ids[match.question_number - 1], True)
            match.fifty_fifty = False
            match.save()
            question = Question.objects.get(
                id=match.question_ids[match.question_number - 1])
            answers = [question.a, question.b, question.c, question.d]
            answers.remove(question.answer)
            answers.pop(random.randint(0, 2))
            return JsonResponse({
                'success': True,
                'wrong_answers': answers,
            },
                                status=200)

        elif change_question:
            if not match.change_question:
                return JsonResponse({'error': 'Lifeline already used'},
                                    status=400)
            match.last_interaction = timezone.now()
            match.change_question = False
            answer = (Question.objects.get(
                id=match.question_ids[match.question_number - 1])).answer
            match.question_ids.pop(match.question_number - 1)
            match.save()
            question = Question.objects.get(
                id=match.question_ids[match.question_number - 1])
            return JsonResponse(
                {
                    'success': True,
                    'correct_answer': answer,
                    'next_question': {
                        'number': match.question_number,
                        'text': question.text,
                        'options':
                        [question.a, question.b, question.c, question.d]
                    },
                },
                status=200)

        elif refill_time:
            if not match.refill_time:
                return JsonResponse({'error': 'Lifeline already used'},
                                    status=400)
            match.refill_time = False
            match.last_interaction = timezone.now()
            match.save()
            return JsonResponse({'success': True}, status=200)

        return JsonResponse({'error': 'No lifeline specified'}, status=400)

    except Exception as e:
        return JsonResponse({'error': str(e)}, status=500)
