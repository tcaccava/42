from django.db import models
from django.utils import timezone
from config import settings


class User(models.Model):
    id = models.AutoField(primary_key=True)
    username = models.CharField(max_length=100, null=True, unique=True)
    intra_id = models.CharField(max_length=255, null=True, unique=True, blank=True)
    email = models.CharField(max_length=100, null=True, unique=True)
    created_at = models.DateTimeField(auto_now_add=True)
    password = models.CharField(max_length=255, null=True)
    emoji = models.CharField(max_length=255, null=True)
    profile_picture = models.ImageField(
        upload_to="profile_pictures/", null=True, blank=True
    )
    is_online = models.BooleanField(default=False)
    last_warning = models.DateTimeField(null=True, blank=True)
    muted_until = models.DateTimeField(null=True, blank=True)

    def __str__(self):
        return self.username

    def warn(self):
        """Warn the user. If the user has been warned recently, mute them.
        Returns True if the user has been muted, False otherwise.
        """
        if (not self.last_warning or self.last_warning < timezone.now() -
                timezone.timedelta(seconds=settings.WARNING_DURATION_SEC)):
            self.last_warning = timezone.now()
            self.save()
            return False

        self.muted_until = timezone.now() + timezone.timedelta(
            seconds=settings.MUTE_DURATION_SEC)

        self.save()
        return True

    def is_muted(self):
        return self.muted_until and self.muted_until > timezone.now()


class Friends(models.Model):
    STATUS_CHOICES = [
        ("pending", "In attesa"),
        ("accepted", "Accettato"),
        ("declined", "Rifiutato"),
        ("expired", "Scaduto"),
    ]
    id = models.AutoField(primary_key=True)
    user1 = models.ForeignKey(
        User, on_delete=models.CASCADE, related_name="friend_requests_sent"
    )
    user2 = models.ForeignKey(
        User, on_delete=models.CASCADE, related_name="friend_requests_received"
    )
    status = models.CharField(max_length=100, choices=STATUS_CHOICES, default="pending")
    created_at = models.DateTimeField(auto_now_add=True)

    def get_friends(user):
        user_friend1 = Friends.objects.filter(user1=user).values("user2_id")
        user_friend2 = Friends.objects.filter(user2=user).values("user1_id")
        return list(user_friend1) + list(user_friend2)

    def __str__(self):
        return f"{self.user1.username} - {self.user2.username}"


class Badge(models.Model):
    id = models.AutoField(primary_key=True)
    name = models.CharField(max_length=100, null=True)
    description = models.CharField(max_length=255, null=True)
    image = models.CharField(max_length=255, null=True, blank=True)
    condition = models.CharField(max_length=255, null=True)
    xp = models.IntegerField(default=0)
    use = models.CharField(max_length=100, null=True)

    def __str__(self):
        return self.name
