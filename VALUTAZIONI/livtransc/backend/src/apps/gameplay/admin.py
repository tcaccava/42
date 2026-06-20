from django.contrib import admin

from .models import SinglePlayerMatch, Question, OldMatches, LocalMultiplayer

# Register your models here.

admin.site.register(SinglePlayerMatch)
admin.site.register(Question)
admin.site.register(OldMatches)
admin.site.register(LocalMultiplayer)
