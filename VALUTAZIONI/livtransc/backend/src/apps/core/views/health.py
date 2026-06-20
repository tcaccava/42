from django.http import JsonResponse


def health(request):
    if request.method != "GET":
        return JsonResponse({"error": "Method not allowed"}, status=405)

    return JsonResponse({"success": True}, status=200)
