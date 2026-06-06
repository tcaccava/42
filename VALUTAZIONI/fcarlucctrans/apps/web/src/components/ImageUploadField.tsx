'use client';

import { useId, useState, type DragEvent } from 'react';
import { FormLabel, dropZoneClassName } from '@/components/ui/form';
import { local } from '@/utilities';
import { compressImageFile } from '@/lib/compress-image';

const MAX_FILE_SIZE = 2 * 1024 * 1024;
const ALLOWED_MIME = ['image/png', 'image/jpeg', 'image/webp', 'image/gif'];

type ImageUploadFieldProps = {
  label?: string;
  value: string;
  onChange: (value: string) => void;
  inputId?: string;
  previewClassName?: string;
  showPreview?: boolean;
  compact?: boolean;
};

function fileToDataUrl(file: File): Promise<string> {
  return compressImageFile(file);
}

export function ImageUploadField({
  label,
  value,
  onChange,
  inputId,
  previewClassName = '',
  showPreview = true,
  compact = false,
}: ImageUploadFieldProps) {
  const autoId = useId();
  const id = inputId ?? `image-upload-${autoId}`;
  const [error, setError] = useState('');
  const t = local.useLocalization().t;

  async function handleFiles(fileList: FileList | null) {
    if (!fileList?.length) return;
    const file = fileList[0];
    if (file.size > MAX_FILE_SIZE) {
      setError('File is too big. Max 2 MB.');
      return;
    }
    if (!ALLOWED_MIME.includes(file.type)) {
      setError('Unsupported file format. Use only PNG/JPEG/WebP/GIF.');
      return;
    }
    setError('');
    onChange(await fileToDataUrl(file));
  }

  return (
    <div>
      {label ? <FormLabel htmlFor={id}>{label}</FormLabel> : null}
      <div
        onDragOver={(e) => e.preventDefault()}
        onDrop={(e: DragEvent) => {
          e.preventDefault();
          void handleFiles(e.dataTransfer?.files ?? null);
        }}
        className={`${dropZoneClassName} ${compact ? 'p-4' : ''}`}
        onClick={() => document.getElementById(id)?.click()}
      >
        <div className="text-sm text-gray-500">{t.dragDropScreens}.</div>
      </div>
      <input
        id={id}
        type="file"
        accept="image/png,image/jpeg,image/webp,image/gif"
        className="hidden"
        onChange={(e) => void handleFiles(e.target.files)}
      />
      {error ? <p className="mt-1 text-sm text-red-600">{error}</p> : null}
      {showPreview && value ? (
        <div className={`mt-3 overflow-hidden rounded-lg border border-gray-200 ${previewClassName}`}>
          {/* eslint-disable-next-line @next/next/no-img-element */}
          <img src={value} alt="Preview" className="h-auto w-full object-cover" />
        </div>
      ) : null}
      {value ? (
        <button
          type="button"
          className="mt-2 text-sm text-red-600 hover:underline"
          onClick={() => onChange('')}
        >
          Remove image
        </button>
      ) : null}
    </div>
  );
}
