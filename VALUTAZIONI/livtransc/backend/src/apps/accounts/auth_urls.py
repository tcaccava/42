from django.urls import path

from .views import auth

urlpatterns = [
    path("register", auth.register, name="api-register"),
    path("login", auth.login, name="api-login"),
    path("logout", auth.logout, name="api-logout"),
    path("oauth/callback", auth.oauth_callback, name="api-oauth-callback"),
]
