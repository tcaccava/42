-- CreateTable
CREATE TABLE "User" (
    "id" SERIAL NOT NULL,
    "uuid" UUID NOT NULL,
    "login" TEXT NOT NULL,
    "email" TEXT NOT NULL,
    "image" TEXT,
    "first_name" TEXT,
    "last_name" TEXT,
    "displayname" TEXT,
    "pool_month" TEXT,
    "pool_year" INTEGER,
    "location" TEXT,
    "wallet" INTEGER,
    "correction_point" INTEGER,
    "grade" TEXT,
    "kind" TEXT,
    "status" TEXT,
    "createdAt" TIMESTAMP(3) NOT NULL DEFAULT CURRENT_TIMESTAMP,
    "updatedAt" TIMESTAMP(3) NOT NULL,

    CONSTRAINT "User_pkey" PRIMARY KEY ("id")
);

-- CreateIndex
CREATE UNIQUE INDEX "User_uuid_key" ON "User"("uuid");

-- CreateIndex
CREATE UNIQUE INDEX "User_login_key" ON "User"("login");
