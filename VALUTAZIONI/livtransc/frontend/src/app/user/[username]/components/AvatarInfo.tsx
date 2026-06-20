interface AvatarInfoProps {
  avatar: string;
  profilePicture?: string | null;
  username: string;
  level: number;
  xp: number;
  xpMax: number;
}

export default function AvatarInfo({ avatar, profilePicture, username, level, xp, xpMax }: AvatarInfoProps) {
  return (
    <div className="flex w-full gap-8 items-center justify-center">
      <div className="flex flex-col items-center">
        <div className="w-32 h-32 rounded-full bg-[#334155] flex items-center justify-center text-5xl mb-2 border-4 border-[#FACC15]">
          {profilePicture ? (
            <img src={profilePicture} alt="Profile avatar" className="w-full h-full rounded-full object-cover" />
          ) : (
            <span role="img" aria-label="avatar">{avatar}</span>
          )}
        </div>
      </div>
      <div className="flex flex-col gap-2 items-start">
        <span className="text-2xl font-bold">{username}</span>
        <span className="text-[#FACC15] font-semibold">Level {level}</span>
        <span className="text-[#94A3B8]">XP: {xp} / {xpMax}</span>
      </div>
    </div>
  );
}
