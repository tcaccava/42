'use client';

import { Event } from '@/types';
import { Calendar, Clock, MapPin, Users } from 'lucide-react';
import { Button } from '@/components/Button';
import { local } from '@/utilities';
import { SectionCard } from '@/components/ui/layout';

interface EventCardProps {
  event: Event;
  currentUser: string;
  onJoin: (eventId: string) => void;
  onLeave: (eventId: string) => void;
  disabled?: boolean;
}

export default function EventCard({
  event,
  currentUser,
  onJoin,
  onLeave,
  disabled = false,
}: EventCardProps) {
  const isParticipant = event.participants.includes(currentUser);
  const isFull = event.participants.length >= event.maxParticipants;
  const t = local.useLocalization().t;

  const handleAction = () => {
    if (isParticipant) {
      onLeave(event.id);
    } else if (!isFull) {
      onJoin(event.id);
    }
  };

  return (
    <SectionCard contentClassName="space-y-0 overflow-hidden p-0">
      {event.coverImage ? (
        // eslint-disable-next-line @next/next/no-img-element
        <img
          src={event.coverImage}
          alt=""
          className="h-40 w-full object-cover"
        />
      ) : null}
      <div className="space-y-4 p-6">
      <div>
        <h3 className="text-xl font-bold text-gray-900 mb-2">{event.title}</h3>
        <div className="flex flex-wrap items-center gap-2 mb-3">
          <img
            src={event.organizerImage}
            alt={event.organizer}
            className="h-6 w-6 rounded-full object-cover"
          />
          <span className="text-sm text-gray-600">
            {t.organizedBy}<span className="font-semibold text-gray-900">{event.organizer}</span>
          </span>
          <span className="rounded-full bg-teal-50 px-2 py-0.5 text-xs font-medium text-teal-700">
            {event.campus}
          </span>
        </div>
      </div>

      <p className="text-gray-700">{event.description}</p>

      <div className="space-y-2">
        <div className="flex items-center gap-3 text-gray-700">
          <Calendar className="h-5 w-5 text-teal-600" aria-hidden="true" />
          <span className="text-sm">{event.date}</span>
        </div>
        <div className="flex items-center gap-3 text-gray-700">
          <Clock className="h-5 w-5 text-teal-600" aria-hidden="true" />
          <span className="text-sm">{event.time}</span>
        </div>
        <div className="flex items-center gap-3 text-gray-700">
          <MapPin className="h-5 w-5 text-teal-600" aria-hidden="true" />
          <span className="text-sm">{event.location}</span>
        </div>
        <div className="flex items-center gap-3 text-gray-700">
          <Users className="h-5 w-5 text-teal-600" aria-hidden="true" />
          <span className="text-sm">
            {event.participants.length} / {event.maxParticipants} {t.participants}
          </span>
        </div>
      </div>

      {event.tags.length > 0 && (
        <div className="flex flex-wrap gap-2">
          {event.tags.map((tag, index) => (
            <span
              key={index}
              className="rounded-full bg-gray-100 px-3 py-1 text-xs font-medium text-gray-700"
            >
              {tag}
            </span>
          ))}
        </div>
      )}

      <Button
        onClick={handleAction}
        disabled={disabled || (!isParticipant && isFull)}
        variant={isParticipant ? 'secondary' : 'primary'}
        className="w-full"
        aria-label={isParticipant ? `Leave ${event.title}` : isFull ? `Event ${event.title} is full` : `Join ${event.title}`}
      >
        {isParticipant ? t.leaveEvent : isFull ? t.eventFull : disabled ? '…' : t.joinEvent}
      </Button>
      </div>
    </SectionCard>
  );
}
