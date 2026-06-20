from django.db import models

from apps.accounts.models import Badge, User

import uuid as uuid_lib


class Statistic(models.Model):
    id = models.AutoField(primary_key=True)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    games = models.IntegerField(default=0)
    xp = models.IntegerField(default=0)
    correct_answers = models.IntegerField(default=0)
    wrong_answers = models.IntegerField(default=0)
    badges = models.ManyToManyField(Badge)
    consecutive_correct_answers = models.IntegerField(default=0)
    highest_streak = models.IntegerField(default=0)
    lifeline_used = models.IntegerField(default=0)
    
    single_player_games = models.IntegerField(default=0)
    victories = models.IntegerField(default=0)
    lost = models.IntegerField(default=0)
    sp_correct_answers = models.IntegerField(default=0) 
    wins_in_a_row = models.IntegerField(default=0)
    
    multiplayer_games = models.IntegerField(default=0)
    tournament_wins = models.IntegerField(default=0)
    mp_correct_answers = models.IntegerField(default=0)

    def __str__(self):
        return f"{self.user.username} statistics"


class Notification(models.Model):
    STATUS_CHOICES = [
        ('pending', 'In attesa'),
        ('accepted', 'Accettato'),
        ('badge', 'Badge'),
        ('read', 'Letto'),
    ]
    id = models.AutoField(primary_key=True)
    receiving = models.ForeignKey(User,
                                  on_delete=models.CASCADE,
                                  related_name="user_got_notificated")
    created_at = models.DateTimeField(auto_now_add=True)
    text = models.CharField(max_length=255, null=True)
    status = models.CharField(max_length=100,
                              choices=STATUS_CHOICES,
                              null=True)
    event_id = models.CharField(max_length=255, null=True, blank=True)

    class Meta:
        ordering = ['-created_at']

    def __str__(self):
        return f"{self.receiving} - {self.text}"


class Message(models.Model):
    uuid = models.UUIDField(db_index=True,
                            default=uuid_lib.uuid4,
                            editable=False)
    author = models.ForeignKey(User,
                               on_delete=models.CASCADE,
                               related_name='sent_messages',
                               null=True,
                               blank=True)
    content = models.TextField(max_length=500)
    system = models.BooleanField(default=False)
    timestamp = models.DateTimeField(auto_now_add=True)

    class Meta:
        ordering = ["-timestamp"]

    @classmethod
    def latest_for_chat(cls, limit):
        return list(reversed(cls.objects.all()[:limit]))

    def __str__(self):
        if self.system:
            return f"System message at {self.timestamp}"
        return f"Message from {self.author.username if self.author else 'Unknown' } at {self.timestamp}"
