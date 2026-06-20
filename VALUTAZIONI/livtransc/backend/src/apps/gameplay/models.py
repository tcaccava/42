from django.db import models

from apps.accounts.models import User


class SinglePlayerMatch(models.Model):
    id = models.AutoField(primary_key=True)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    started_at = models.DateTimeField(auto_now_add=True)
    question_ids = models.JSONField(default=list)
    question_number = models.IntegerField(default=1)
    last_interaction = models.DateTimeField(null=True, blank=True)
    fifty_fifty = models.BooleanField(default=True)
    change_question = models.BooleanField(default=True)
    double_chance = models.BooleanField(default=True)
    refill_time = models.BooleanField(default=True)

    def __str__(self):
        return f"{self.user.username} started at {self.started_at}"


class Question(models.Model):
    id = models.AutoField(primary_key=True)
    text = models.CharField(max_length=1000, null=True)
    a = models.CharField(max_length=1000, null=True)
    b = models.CharField(max_length=1000, null=True)
    c = models.CharField(max_length=1000, null=True)
    d = models.CharField(max_length=1000, null=True)
    answer = models.CharField(max_length=1000, null=True)

    def __str__(self):
        return self.text


class OldMatches(models.Model):
    id = models.AutoField(primary_key=True)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    finished_at = models.DateTimeField(auto_now_add=True)
    status = models.IntegerField(default=0)
    mode = models.CharField(max_length=100, null=True)
    correct_answers = models.IntegerField(default=0)
    wrong_answers = models.IntegerField(default=0)
    ranking = models.IntegerField(default=0, null=True)

    def __str__(self):
        return f"{self.user.username} finished at {self.finished_at}"


class LocalMultiplayer(models.Model):
    STATUS_CHOICES = [
        ('waiting', 'In attesa'),
        ('playing', 'In gioco'),
        ('finished', 'Finita'),
    ]
    id = models.AutoField(primary_key=True)
    users = models.JSONField(default=list)
    code = models.CharField(max_length=10, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    question_ids = models.JSONField(default=list)
    question_number = models.IntegerField(default=0)
    status = models.CharField(max_length=255, default='waiting', null=True)
    current_answers = models.JSONField(default=dict)

    def __str__(self):
        return self.code


class Multiplayer(models.Model):
    STATUS_CHOICES = [
        ('waiting', 'In attesa'),
        ('playing', 'In gioco'),
        ('finished', 'Finita'),
    ]
    id = models.AutoField(primary_key=True)
    users = models.JSONField(default=list)
    code = models.CharField(max_length=10, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    question_ids = models.JSONField(default=list)
    question_number = models.IntegerField(default=0)
    status = models.CharField(max_length=255, default='waiting', null=True)
    current_answers = models.JSONField(default=dict)

    def __str__(self):
        return self.code
