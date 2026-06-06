import { BadRequestException, Injectable } from '@nestjs/common';
import { UserProfileRepository } from './database/user-profile.repository';

export type EditableProfileFields = {
  bio?: string;
  username?: string;
  website?: string;
  github?: string;
  linkedin?: string;
  campus?: string;
  coverImage?: string;
};

@Injectable()
export class AppService {
  constructor(private readonly userProfileRepository: UserProfileRepository) {}

  async getProfileByUserId(userId: string, login: string) {
  if (!userId || typeof userId !== 'string') {
    throw new BadRequestException('userId is required');
  }
  if (!login || typeof login !== 'string') {
    throw new BadRequestException('login is required');
  }
  const profile = await this.userProfileRepository.findByUserId(userId);
  return { status: 'success', profile: profile ?? { userId, login } };
  }

  async upsertProfileByUserId(userId: string, login: string, profile: EditableProfileFields) {
  if (!userId || typeof userId !== 'string') {
    throw new BadRequestException('userId is required');
  }
  if (!login || typeof login !== 'string') {
    throw new BadRequestException('login is required');
  }
  const updated = await this.userProfileRepository.upsertByUserId(userId, login, profile);
  return { status: 'success', profile: updated };
  }
}
