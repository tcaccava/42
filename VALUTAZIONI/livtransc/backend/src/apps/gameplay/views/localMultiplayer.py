from django.shortcuts import render
import json
import string
from django.http import JsonResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.exceptions import ObjectDoesNotExist
from django.db.models import Q

from apps.gameplay.models import LocalMultiplayer, Question
import random
from django.utils import timezone


def questions(n):
    question_ids = list(Question.objects.values_list('id', flat=True))
    numbers = random.sample(question_ids, n)
    return (numbers)


@csrf_exempt
def create_LMP_game(request):
    if (request.method != 'POST'):
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    question_ids = questions(15)
    code = "".join(random.choices(string.digits, k=4))
    while (code in LocalMultiplayer.objects.values_list('code')):
        code = "".join(random.choices(string.digits, k=4))
    new_localMP = LocalMultiplayer(code=code, question_ids=question_ids)
    new_localMP.save()
    return JsonResponse(
        {
            'success': True,
            'match_id': new_localMP.id,
            'code': new_localMP.code,
        },
        status=200)


@csrf_exempt
def get_MP_players(request):
    if (request.method != 'GET'):
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    code = request.GET.get('code')
    if not code:
        return JsonResponse({'error': 'missing field'}, status=400)
    try:
        MP_match = LocalMultiplayer.objects.only(
            'users', 'status', 'question_number').get(code=code)
    except:
        return JsonResponse({'error': 'match not found'}, status=404)

    return JsonResponse(
        {
            'success': True,
            'players': MP_match.users,
            'status': MP_match.status,
            'question_number': MP_match.question_number
        },
        status=200)


@csrf_exempt
def add_MP_player(request):
    print(
        f"[DEBUG] add_MP_player called - Method: {request.method}, Headers: {dict(request.headers)}"
    )
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    try:
        body = json.loads(request.body)
        print(f"[DEBUG] Request body: {body}")
    except Exception as e:
        print(f"[DEBUG] JSON parse error: {e}")
        return JsonResponse({'error': 'Invalid Json'}, status=402)

    username = body.get('username', '').strip()
    code = body.get('roomcode', '').strip()
    if not username or not code:
        return JsonResponse({'error': 'missing field'}, status=400)
    try:
        current_match = LocalMultiplayer.objects.get(code=code)
    except:
        return JsonResponse({'error': 'match not found'}, status=404)

    if username not in current_match.users:
        current_match.users.append(username)
        current_match.save()
    else:
        return JsonResponse({'error': 'username already taken'}, status=401)
    return JsonResponse({'success': True}, status=200)


@csrf_exempt
def submit_answer(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    try:
        data = json.loads(request.body)
        code = data.get('roomcode')
        answer = data.get('answer')
        username = data.get('username')
        if not code or not answer or not username:
            return JsonResponse({'error': 'Missing field'}, status=401)
    except:
        return JsonResponse({'error': 'Invalid Json'}, status=500)

    try:
        match = LocalMultiplayer.objects.get(code=code)
        if username in match.users:
            match.current_answers[username] = answer
            match.save()
            return JsonResponse({'success': True}, status=200)
        else:
            return JsonResponse({'error': 'User not playing'}, status=402)
    except:
        return JsonResponse({'error': 'Question not found'}, status=404)


@csrf_exempt
def get_answers(request):
    if request.method != 'GET':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    code = request.GET.get('roomcode')
    if not code:
        return JsonResponse({'error': 'Missing field'}, status=400)

    try:
        match = LocalMultiplayer.objects.only('current_answers').get(code=code)
        return JsonResponse({
            'success': True,
            'answers': match.current_answers
        },
                            status=200)
    except:
        return JsonResponse({'error': 'Match not found'}, status=404)


@csrf_exempt
def get_next_question(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    try:
        data = json.loads(request.body)
        code = data.get('roomcode')
        if not code:
            return JsonResponse({'error': 'Missing field'}, status=401)
    except:
        return JsonResponse({'error': 'Invalid Json'}, status=500)

    try:
        match = LocalMultiplayer.objects.get(code=code)
    except:
        return JsonResponse({'error': 'Match not found'}, status=404)

    if match.status == 'playing':
        match.question_number += 1
        match.current_answers = {}
        match.save()

        if match.question_number == 15:
            match.status = 'finished'
            match.save()
            return JsonResponse({
                'success': True,
                'finished': True
            },
                                status=200)

        next_question = Question.objects.get(
            id=match.question_ids[match.question_number])
        return JsonResponse(
            {
                'success': True,
                'question_id': next_question.id,
                'next_question': {
                    'number':
                    match.question_number + 1,
                    'text':
                    next_question.text,
                    'options': [
                        next_question.a, next_question.b, next_question.c,
                        next_question.d
                    ],
                    'answer':
                    next_question.answer
                }
            },
            status=200)
    else:
        match.status = 'playing'
        match.current_answers = {}
        match.question_number = 0
        match.save()
        question = Question.objects.get(id=match.question_ids[0])
        return JsonResponse(
            {
                'success': True,
                'question_id': question.id,
                'next_question': {
                    'number': match.question_number + 1,
                    'text': question.text,
                    'options':
                    [question.a, question.b, question.c, question.d],
                    'answer': question.answer
                }
            },
            status=200)


@csrf_exempt
def check_session_local(request):
    if request.method != 'GET':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    code = request.GET.get('roomcode')
    if code:
        try:
            match = LocalMultiplayer.objects.get(code=code)
            playing = match.status == 'playing'
            question = Question.objects.get(
                id=match.question_ids[match.question_number])
            return JsonResponse(
                {
                    'success': True,
                    'existing': True,
                    'playing': playing,
                    'match_id': match.id,
                    'next_question': {
                        'number': match.question_number + 1,
                        'text': question.text,
                        'options':
                        [question.a, question.b, question.c, question.d],
                        'answer': question.answer
                    }
                },
                status=200)
        except:
            return JsonResponse({'error': 'match not found'}, status=404)
    return JsonResponse({'success': True, 'existing': False}, status=200)


@csrf_exempt
def reset_LMP_game(request):
    if request.method != 'POST':
        return JsonResponse({'error': 'Method not allowed'}, status=405)

    try:
        data = json.loads(request.body)
    except:
        return JsonResponse({'error': 'Invalid Json'}, status=400)

    code = data.get('roomcode')
    try:
        match = LocalMultiplayer.objects.get(code=code)
        match.status = 'waiting'
        match.question_ids.clear()
        match.question_ids = questions(15)
        match.current_answers = {}
        match.question_number = 0
        match.save()
        return JsonResponse({'success': True}, status=200)
    except:
        return JsonResponse({'error': 'Match not found'}, status=404)
