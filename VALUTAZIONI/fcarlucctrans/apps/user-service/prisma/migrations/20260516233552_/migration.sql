-- CreateTable
CREATE TABLE "UserProfile" (
    "userId" UUID NOT NULL,
    "login" TEXT NOT NULL,
    "bio" TEXT,
    "username" TEXT,
    "website" TEXT,
    "github" TEXT,
    "linkedin" TEXT,
    "campus" TEXT,
    "createdAt" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "updatedAt" TIMESTAMP(3) NOT NULL,

    CONSTRAINT "UserProfile_pkey" PRIMARY KEY ("userId")
);

-- CreateIndex
CREATE UNIQUE INDEX "UserProfile_login_key" ON "UserProfile"("login");
