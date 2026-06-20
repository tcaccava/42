from django.contrib import admin

from apps.accounts.models import Friends, User, Badge


# Register your models here.
class UserAdmin(admin.ModelAdmin):
    list_display = ['id', 'username', 'email', 'created_at']
    search_fields = ['username', 'email']
    list_filter = ['created_at']


class FriendsAdmin(admin.ModelAdmin):
    list_display = ['id', 'user1', 'user2', 'status', 'created_at']
    search_fields = ['user1__username', 'user2__username']
    list_filter = ['status', 'created_at']
    # Mostra i campi di relazione in forma leggibile
    readonly_fields = ['created_at']


admin.site.register(User, UserAdmin)
admin.site.register(Friends, FriendsAdmin)
admin.site.register(Badge)
