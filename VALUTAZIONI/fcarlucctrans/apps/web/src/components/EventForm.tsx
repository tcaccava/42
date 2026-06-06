'use client';

import { useState } from 'react';
import { User } from '@/types';
import { Calendar, X } from 'lucide-react';
import Image from 'next/image';
import { Input, Textarea } from '@/components/Input';
import { Button } from '@/components/Button';
import { Badge } from '@/components/Badge';
import { local } from '@/utilities';
import { PageHeader, PageShell, SectionCard } from '@/components/ui/layout';
import { FormLabel } from '@/components/ui/form';
import { ImageUploadField } from '@/components/ImageUploadField';

export type EventFormValues = {
  title: string;
  description: string;
  date: string;
  time: string;
  location: string;
  maxParticipants: number;
  tags: string[];
  coverImage: string;
};

type EventFormProps = {
  currentUser: User;
  mode: 'create' | 'edit';
  initialValues?: Partial<EventFormValues>;
  minParticipants?: number;
  isSubmitting?: boolean;
  onSubmit: (values: EventFormValues) => void | Promise<void>;
  onCancel: () => void;
};

export function EventForm({
  currentUser,
  mode,
  initialValues,
  minParticipants = 1,
  isSubmitting = false,
  onSubmit,
  onCancel,
}: EventFormProps) {
  const [title, setTitle] = useState(initialValues?.title ?? '');
  const [description, setDescription] = useState(initialValues?.description ?? '');
  const [date, setDate] = useState(initialValues?.date ?? '');
  const [time, setTime] = useState(initialValues?.time ?? '');
  const [location, setLocation] = useState(initialValues?.location ?? '');
  const [maxParticipants, setMaxParticipants] = useState(initialValues?.maxParticipants ?? 10);
  const [tagInput, setTagInput] = useState('');
  const [tags, setTags] = useState<string[]>(initialValues?.tags ?? []);
  const [coverImage, setCoverImage] = useState(initialValues?.coverImage ?? '');
  const t = local.useLocalization().t;

  const handleAddTag = () => {
    if (tagInput.trim() && !tags.includes(tagInput.trim())) {
      setTags([...tags, tagInput.trim()]);
      setTagInput('');
    }
  };

  const handleRemoveTag = (tagToRemove: string) => {
    setTags(tags.filter((tag) => tag !== tagToRemove));
  };

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (isSubmitting) return;
    if (title && description && date && time && location) {
      await onSubmit({
        title,
        description,
        date,
        time,
        location,
        maxParticipants,
        tags,
        coverImage,
      });
    }
  };

  const isEdit = mode === 'edit';

  return (
    <PageShell maxWidth="2xl">
      <PageHeader
        icon={<Calendar className="h-5 w-5 text-teal-600" />}
        title={isEdit ? 'Edit event' : t.createEvent}
        description={
          isEdit
            ? 'Update your event details. Registrations are kept.'
            : 'Organize a campus event and invite other students.'
        }
      />

      <SectionCard>
        <form onSubmit={handleSubmit} className="space-y-4">
          <div className="flex items-center gap-3 border-b border-gray-200 pb-4">
            <Image
              src={currentUser.image || '/globe.svg'}
              alt={currentUser.login}
              width={40}
              height={40}
              className="h-10 w-10 rounded-full object-cover"
            />
            <div>
              <p className="font-medium text-gray-900">{currentUser.login}</p>
              <p className="text-sm text-gray-600">{currentUser.campus}</p>
            </div>
          </div>

          <Input
            label={t.eventTitle}
            id="title"
            type="text"
            placeholder={t.eventTitlePlaceholder}
            value={title}
            onChange={(e) => setTitle(e.target.value)}
            required
          />

          <Textarea
            label="Description"
            id="description"
            placeholder={t.eventDescriptionPlaceholder}
            value={description}
            onChange={(e) => setDescription(e.target.value)}
            rows={4}
            required
          />

          <ImageUploadField
            label={t.screenshot}
            value={coverImage}
            onChange={setCoverImage}
            previewClassName="max-h-48"
          />

          <div className="grid grid-cols-2 gap-4">
            <Input
              label={t.date}
              id="date"
              type="date"
              value={date}
              onChange={(e) => setDate(e.target.value)}
              required
            />
            <Input
              label={t.time}
              id="time"
              type="time"
              value={time}
              onChange={(e) => setTime(e.target.value)}
              required
            />
          </div>

          <Input
            label={t.location}
            id="location"
            type="text"
            placeholder={t.locationPlaceholder}
            value={location}
            onChange={(e) => setLocation(e.target.value)}
            required
          />

          <Input
            label={t.maxParticipants}
            id="maxParticipants"
            type="number"
            min={minParticipants}
            value={maxParticipants}
            onChange={(e) => setMaxParticipants(parseInt(e.target.value, 10) || 10)}
            required
          />

          <div>
            <FormLabel>Tags</FormLabel>
            <div className="flex gap-2">
              <Input
                type="text"
                placeholder={t.addTagPlaceholder}
                value={tagInput}
                onChange={(e) => setTagInput(e.target.value)}
                onKeyDown={(e) => e.key === 'Enter' && (e.preventDefault(), handleAddTag())}
              />
              <Button type="button" onClick={handleAddTag} variant="secondary">
                {t.add}
              </Button>
            </div>
            {tags.length > 0 && (
              <div className="mt-3 flex flex-wrap gap-2">
                {tags.map((tag) => (
                  <Badge key={tag} variant="tag">
                    {tag}
                    <button
                      type="button"
                      onClick={() => handleRemoveTag(tag)}
                      className="ml-1 hover:text-red-600"
                    >
                      <X className="h-3 w-3" />
                    </button>
                  </Badge>
                ))}
              </div>
            )}
          </div>

          <div className="flex flex-col gap-3 sm:flex-row">
            <Button type="submit" className="w-full sm:flex-1" disabled={isSubmitting}>
              <Calendar className="mr-2 h-4 w-4" />
              {isSubmitting
                ? isEdit
                  ? 'Saving…'
                  : 'Creating…'
                : isEdit
                  ? 'Save changes'
                  : t.createEvent}
            </Button>
            <Button
              type="button"
              variant="secondary"
              className="w-full sm:flex-1"
              onClick={onCancel}
              disabled={isSubmitting}
            >
              Cancel
            </Button>
          </div>
        </form>
      </SectionCard>
    </PageShell>
  );
}
