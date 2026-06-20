FROM python:3.12-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

RUN apt-get update && apt-get install -y curl && rm -rf /var/lib/apt/lists/*

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

RUN python -c "from detoxify import Detoxify; Detoxify('multilingual')"

COPY src/ ./src/

WORKDIR /app/src

EXPOSE 8000

CMD python manage.py collectstatic --noinput && python manage.py migrate --noinput && python -m populate_db.addUsers && python -m populate_db.addQuestions && python -m populate_db.addBadges && python manage.py runserver 0.0.0.0:8000
