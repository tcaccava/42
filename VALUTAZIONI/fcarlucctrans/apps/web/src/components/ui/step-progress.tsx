type StepProgressBarProps = {
  steps: string[];
  currentStep: number;
  /** 0–1 optional overlay while uploading on the final step */
  uploadProgress?: number | null;
};

export function StepProgressBar({
  steps,
  currentStep,
  uploadProgress = null,
}: StepProgressBarProps) {
  const stepCount = steps.length;
  const baseProgress = Math.min(currentStep / stepCount, 1);
  const barProgress =
    uploadProgress != null && uploadProgress > 0
      ? baseProgress + uploadProgress / stepCount
      : baseProgress;

  return (
    <div className="mb-8 space-y-4">
      <div className="flex items-center justify-between gap-2">
        {steps.map((label, index) => {
          const stepNumber = index + 1;
          const isActive = stepNumber === currentStep;
          const isComplete = stepNumber < currentStep;

          return (
            <div key={label} className="flex flex-1 flex-col items-center gap-2 text-center">
              <div
                className={`flex h-9 w-9 items-center justify-center rounded-full text-sm font-semibold transition-colors ${
                  isComplete
                    ? 'bg-teal-600 text-white'
                    : isActive
                      ? 'bg-teal-600 text-white ring-4 ring-teal-100'
                      : 'bg-gray-200 text-gray-500'
                }`}
                aria-current={isActive ? 'step' : undefined}
              >
                {stepNumber}
              </div>
              <span
                className={`hidden text-xs font-medium sm:block ${
                  isActive || isComplete ? 'text-teal-700' : 'text-gray-500'
                }`}
              >
                {label}
              </span>
            </div>
          );
        })}
      </div>

      <div className="space-y-1">
        <div
          className="relative h-2 overflow-hidden rounded-full bg-gray-200"
          role="progressbar"
          aria-valuemin={0}
          aria-valuemax={100}
          aria-valuenow={Math.round(barProgress * 100)}
          aria-label={`Step ${currentStep} of ${stepCount}`}
        >
          <div
            className="absolute inset-y-0 left-0 rounded-full bg-gradient-to-r from-teal-500 to-teal-600 transition-all duration-300"
            style={{ width: `${Math.min(barProgress * 100, 100)}%` }}
          />
        </div>
        <p className="text-right text-xs text-gray-500">
          Step {currentStep} of {stepCount}
        </p>
      </div>
    </div>
  );
}
