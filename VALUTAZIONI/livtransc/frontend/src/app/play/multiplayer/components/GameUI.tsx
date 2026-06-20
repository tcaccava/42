"use client";
import { useTranslation } from "@/hooks/useTranslation";
import HintsPanel from "./HintsPanel";

interface Question {
  text: string;
  options: string[];
  number: number;
  total: number;
}

interface GameUIProps {
  question: Question;
  timeRemaining: number;
  onAnswer: (optionIndex: number) => void;
  isLocked: boolean;
  answeredPlayers: number;
  totalPlayers: number;
  optionStatuses?: Record<number, "green" | "red" | "yellow" | "none">;
  isSpectator?: boolean;
  correctAnswers?: number;
  myDisplayName?: string;
  playerAnswers?: Record<string, string>;
  scryingAnswers?: Record<string, string>;
  disabledOptionIndices?: number[];
  hints?: { fifty_fifty: boolean; double_chance: boolean; scrying: boolean };
  onUseHint?: (hintName: string) => void;
}

function OptionButton({
  index,
  text,
  status,
  isLocked,
  onAnswer,
  isSpectator,
  playerAnswers,
  myDisplayName,
  scryingAnswers,
  isDisabled,
}: {
  index: number;
  text: string;
  status: "green" | "red" | "yellow" | "none";
  isLocked: boolean;
  onAnswer: (optionIndex: number) => void;
  isSpectator?: boolean;
  playerAnswers?: Record<string, string>;
  myDisplayName?: string;
  scryingAnswers?: Record<string, string>;
  isDisabled?: boolean;
}) {
  const letter = String.fromCharCode(65 + index);

  const getBgClass = () => {
    if (isDisabled && status !== "red") {
      return "bg-slate-800/50 border-slate-700 opacity-40 cursor-not-allowed";
    }
    switch (status) {
      case "yellow":
        return "bg-[#FACC15]/10 border-[#FACC15]";
      case "green":
        return "bg-green-500/20 border-green-500";
      case "red":
        return "bg-red-500/20 border-red-500";
      default:
        return `bg-[#1E293B] border-transparent ${!isLocked && !isSpectator && !isDisabled ? "hover:bg-[#334155] hover:border-[#FACC15]/40" : ""}`;
    }
  };

  const getLetterBgClass = () => {
    if (isDisabled && status !== "red") {
      return "bg-slate-700 text-slate-500";
    }
    switch (status) {
      case "yellow":
        return "bg-[#FACC15] text-[#0F172A]";
      case "green":
        return "bg-green-500 text-white";
      case "red":
        return "bg-red-500 text-white";
      default:
        return `bg-[#0F172A] text-[#FACC15] ${!isLocked && !isSpectator && !isDisabled ? "group-hover:bg-[#FACC15] group-hover:text-[#0F172A]" : ""}`;
    }
  };

  const getTextClass = () => {
    switch (status) {
      case "yellow":
        return "text-white";
      case "green":
        return "text-green-400";
      case "red":
        return "text-red-400";
      default:
        return "";
    }
  };

  return (
    <button
      onClick={() => !isSpectator && !isDisabled && onAnswer(index)}
      disabled={isLocked || status !== "none" || isSpectator || isDisabled}
      className={`group relative flex-1 flex items-center p-6 border-2 transition-all duration-300 text-left overflow-hidden shadow-xl rounded-2xl
        ${getBgClass()}
        ${isLocked || status !== "none" || isSpectator || isDisabled ? "cursor-default" : "cursor-pointer"}
        ${isSpectator ? "opacity-60" : ""}
      `}
      title={isSpectator ? "Spectators cannot answer" : isDisabled ? "Option eliminated" : ""}
    >
      <span
        className={`w-10 h-10 rounded-xl font-black flex items-center justify-center transition-all duration-300 shadow-inner
          ${getLetterBgClass()}
        `}
      >
        {letter}
      </span>
      <span
        className={`ml-6 font-bold text-xl transition-colors ${getTextClass()} flex-1`}
      >
        {text}
      </span>
      {/* Nickname badges for players who chose this option (rendered inside the option box) */}
      <div className="ml-4 flex flex-wrap gap-2 justify-end max-w-[40%]">
        {Object.entries(playerAnswers || {})
          .filter(([name, ans]) => ans === String.fromCharCode(65 + index) && name !== myDisplayName)
          .map(([name]) => (
            <span
              key={name}
              className="px-3 py-1 bg-[#FACC15] text-[#0F172A] rounded-lg text-[10px] font-black uppercase tracking-wider"
            >
              {name}
            </span>
          ))}
        {/* Scrying badges (indigo) for intel from Scrying hint */}
        {Object.entries(scryingAnswers || {})
          .filter(([, ans]) => ans === String.fromCharCode(65 + index))
          .map(([name]) => (
            <span
              key={`scry-${name}`}
              className="px-3 py-1 bg-indigo-500 text-white rounded-lg text-[10px] font-black uppercase tracking-wider"
            >
              {name}
            </span>
          ))}
      </div>
    </button>
  );
}

export default function GameUI({
  question,
  timeRemaining,
  onAnswer,
  isLocked,
  answeredPlayers,
  totalPlayers,
  optionStatuses = {},
  isSpectator = false,
  correctAnswers = 0,
  playerAnswers = {},
  myDisplayName = "",
  scryingAnswers,
  disabledOptionIndices = [],
  hints,
  onUseHint,
}: GameUIProps) {
  const { t } = useTranslation();
  const timerFraction = timeRemaining / 20;

  return (
    <div className="h-full w-full flex items-center justify-center p-0 bg-[#0F172A] text-white overflow-hidden">
      <div className="relative -top-4 w-full max-w-9xl flex flex-row gap-8 items-center justify-center px-6">
        <div className="flex flex-col items-center gap-6 bg-[#1E293B] p-8 rounded-3xl border border-[#334155] shadow-2xl w-32 shrink-0 animate-in fade-in slide-in-from-left-4 duration-700">
          <span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">
            {t("play.time")}
          </span>

          <div className="relative w-4 h-64 bg-[#0F172A] rounded-full overflow-hidden border border-[#334155] flex flex-col justify-end">
            <div
              className="w-full bg-[#FACC15] transition-all duration-700 shadow-[0_0_15px_#FACC15]"
              style={{ height: `${timerFraction * 100}%` }}
            ></div>
            <div className="absolute top-0 w-full h-px bg-[#FACC15]/30"></div>
          </div>

          <div className="flex flex-col items-center gap-1">
            <span className="text-[#94A3B8] text-[8px] font-black uppercase tracking-[0.2em] text-center">
              {t("play.left")}
            </span>
            <span className="text-4xl font-black text-[#FACC15] tracking-tighter">
              {Math.round(timeRemaining)}
            </span>
          </div>
        </div>

        <div className="flex-1 max-w-3xl flex flex-col gap-8 animate-in fade-in slide-in-from-bottom-4 duration-700">
          <div className="flex flex-col gap-2">
            <span className="text-[#FACC15] font-black tracking-widest text-sm text-center uppercase md:text-left">
              {t("play.questions")} {question.number} {t("play.of_15")}
            </span>
            <h2
              className={`font-black leading-tight text-center md:text-left ${
                question.text.length > 150
                  ? "text-xl md:text-2xl"
                  : "text-2xl md:text-3xl"
              }`}
            >
              {question.text}
            </h2>
          </div>

          <div className="grid grid-cols-1 gap-4">
            {question.options.map((option, i) => (
              <div key={i} className="flex items-start gap-4">
                <OptionButton
                  index={i}
                  text={option}
                  status={optionStatuses[i] || "none"}
                  isLocked={isLocked}
                  onAnswer={onAnswer}
                  isSpectator={isSpectator}
                  playerAnswers={playerAnswers}
                  myDisplayName={myDisplayName}
                  scryingAnswers={scryingAnswers}
                  isDisabled={disabledOptionIndices.includes(i)}
                />
              </div>
            ))}
          </div>
          {isSpectator && (
            <div className="text-center text-[#94A3B8] text-sm font-semibold">
              {t("play.spectating")}
            </div>
          )}
        </div>

        {/* Hints/Lifelines Column */}
        {!isSpectator && hints && onUseHint && (
          <HintsPanel
            hints={hints}
            onUseHint={onUseHint}
            isLocked={isLocked}
            readOnly={false}
            isSpectator={isSpectator}
          />
        )}

        {/* Answers/Correct Answers Score Sidebar */}
        {!isSpectator && (
          <div className="flex flex-col items-center gap-6 bg-[#1E293B] p-8 rounded-3xl border border-[#334155] shadow-2xl w-32 shrink-0 animate-in fade-in slide-in-from-right-4 duration-700">
            <span className="text-[10px] font-bold text-[#475569] uppercase tracking-widest">
              {t("play.answers")}
            </span>

            <div className="relative w-4 h-64 bg-[#0F172A] rounded-full overflow-hidden border border-[#334155] flex flex-col justify-end">
              <div
                className="w-full bg-[#FACC15] transition-all duration-700 shadow-[0_0_15px_#FACC15]"
                style={{
                  height: `${Math.min(100, (correctAnswers / Math.max(1, question.number)) * 100)}%`,
                }}
              ></div>
              <div className="absolute top-0 w-full h-px bg-[#FACC15]/30"></div>
            </div>

            <div className="flex flex-col items-center gap-1">
              <span className="text-[#94A3B8] text-[8px] font-black uppercase tracking-[0.2em] text-center">
                {t("profile.correct_answers")}
              </span>
              <span className="text-4xl font-black text-[#FACC15] tracking-tighter">
                {correctAnswers}
              </span>
              <span className="text-[10px] font-bold text-[#475569]">
                / {question.number}
              </span>
            </div>
          </div>
        )}
      </div>
    </div>
  );
}
