import { Controller, Logger } from '@nestjs/common';
import { MessagePattern, Payload } from '@nestjs/microservices';
import { AppService, EditableProfileFields } from './app.service';
import { ConnectionService } from './connection.service';
import { GdprService } from './gdpr/gdpr.service';
import { PublicApiService } from './public-api/public-api.service';
import { EventService } from './event.service';
import { ConnectionStatus } from './generated/prisma/index.js';

@Controller()
export class AppController {
  private readonly logger = new Logger(AppController.name);

  constructor(
    private readonly appService: AppService,
    private readonly connectionService: ConnectionService,
    private readonly gdprService: GdprService,
    private readonly publicApiService: PublicApiService,
    private readonly eventService: EventService,
  ) {}

  @MessagePattern({ cmd: 'user_profile_get' })
  async getProfile(@Payload() data: { userId: string; login: string }) {
    try {
	  return await this.appService.getProfileByUserId(data.userId, data.login);
    } catch (error) {
      this.logger.error('Error fetching user profile:', error);
      return { status: 'error', message: 'Failed to fetch user profile' };
    }
  }

  @MessagePattern({ cmd: 'user_profile_upsert' })
  async upsertProfile(@Payload() data: { userId: string; login: string; profile: EditableProfileFields }) {
    try {
	  return await this.appService.upsertProfileByUserId(data.userId, data.login, data.profile);
    } catch (error) {
      this.logger.error('Error upserting user profile:', error);
      return { status: 'error', message: 'Failed to update user profile' };
    }
  }

  @MessagePattern({ cmd: 'user_connection_send' })
  async sendConnection(@Payload() data: { requesterId: string; addresseeId: string }) {
    try {
      return await this.connectionService.sendRequest(data.requesterId, data.addresseeId);
    } catch (error: any) {
      this.logger.error('Error sending connection request:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to send connection request',
      };
    }
  }

  @MessagePattern({ cmd: 'user_connection_accept' })
  async acceptConnection(@Payload() data: { userId: string; connectionId: string }) {
    try {
      return await this.connectionService.acceptRequest(data.userId, data.connectionId);
    } catch (error: any) {
      this.logger.error('Error accepting connection:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to accept connection',
      };
    }
  }

  @MessagePattern({ cmd: 'user_connection_reject' })
  async rejectConnection(@Payload() data: { userId: string; connectionId: string }) {
    try {
      return await this.connectionService.rejectRequest(data.userId, data.connectionId);
    } catch (error: any) {
      this.logger.error('Error rejecting connection:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to reject connection',
      };
    }
  }

  @MessagePattern({ cmd: 'user_connection_cancel' })
  async cancelConnection(@Payload() data: { userId: string; connectionId: string }) {
    try {
      return await this.connectionService.cancelRequest(data.userId, data.connectionId);
    } catch (error: any) {
      this.logger.error('Error cancelling connection:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to cancel connection request',
      };
    }
  }

  @MessagePattern({ cmd: 'user_connection_remove' })
  async removeConnection(@Payload() data: { userId: string; connectionId: string }) {
    try {
      return await this.connectionService.removeConnection(data.userId, data.connectionId);
    } catch (error: any) {
      this.logger.error('Error removing connection:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to remove connection',
      };
    }
  }

  @MessagePattern({ cmd: 'user_connection_list' })
  async listConnections(
    @Payload() data: { userId: string; status?: ConnectionStatus },
  ) {
    try {
      return await this.connectionService.listConnections(data.userId, data.status);
    } catch (error: any) {
      this.logger.error('Error listing connections:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to list connections',
      };
    }
  }

  @MessagePattern({ cmd: 'user_connection_status' })
  async connectionStatus(@Payload() data: { userId: string; otherUserId: string }) {
    try {
      return await this.connectionService.getConnectionStatus(data.userId, data.otherUserId);
    } catch (error: any) {
      this.logger.error('Error fetching connection status:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to fetch connection status',
      };
    }
  }

  @MessagePattern({ cmd: 'user_connection_count' })
  async connectionCount(@Payload() data: { userId: string }) {
    try {
      return await this.connectionService.getConnectionCount(data.userId);
    } catch (error: any) {
      this.logger.error('Error counting connections:', error);
      return {
        status: 'error',
        message: error?.message ?? 'Failed to count connections',
      };
    }
  }

  @MessagePattern({ cmd: 'gdpr_export_user' })
  async gdprExportUser(@Payload() data: { userId: string }) {
    try {
      return await this.gdprService.exportUserData(data.userId);
    } catch (error: any) {
      this.logger.error('Error exporting user data:', error);
      return { status: 'error', message: error?.message ?? 'Failed to export user data' };
    }
  }

  @MessagePattern({ cmd: 'gdpr_delete_user' })
  async gdprDeleteUser(@Payload() data: { userId: string }) {
    try {
      return await this.gdprService.deleteUserData(data.userId);
    } catch (error: any) {
      this.logger.error('Error deleting user data:', error);
      return { status: 'error', message: error?.message ?? 'Failed to delete user data' };
    }
  }

  @MessagePattern({ cmd: 'gdpr_anonymize_user' })
  async gdprAnonymizeUser(@Payload() data: { userId: string; newLogin: string }) {
    try {
      return await this.gdprService.anonymizeUserData(data.userId, data.newLogin);
    } catch (error: any) {
      this.logger.error('Error anonymizing user data:', error);
      return { status: 'error', message: error?.message ?? 'Failed to anonymize user data' };
    }
  }

  @MessagePattern({ cmd: 'event_list' })
  async listEvents() {
    try {
      return await this.eventService.listEvents();
    } catch (error: any) {
      this.logger.error('Error listing events:', error);
      return { status: 'error', message: error?.message ?? 'Failed to list events' };
    }
  }

  @MessagePattern({ cmd: 'event_search' })
  async searchEvents(
    @Payload() data: { query?: string; pageNumber?: number; pageSize?: number },
  ) {
    try {
      return await this.eventService.searchEvents(
        data.query ?? '',
        data.pageNumber ?? 0,
        data.pageSize ?? 6,
      );
    } catch (error: any) {
      this.logger.error('Error searching events:', error);
      return { status: 'error', message: error?.message ?? 'Failed to search events' };
    }
  }

  @MessagePattern({ cmd: 'event_get' })
  async getEvent(@Payload() data: { id: string }) {
    try {
      return await this.eventService.getEventById(data.id);
    } catch (error: any) {
      this.logger.error('Error fetching event:', error);
      return { status: 'error', message: error?.message ?? 'Failed to fetch event' };
    }
  }

  @MessagePattern({ cmd: 'event_create' })
  async createEvent(
    @Payload()
    data: {
      userId: string;
      login: string;
      input: {
        title: string;
        description: string;
        organizerImage?: string;
        campus: string;
        date: string;
        time: string;
        location: string;
        maxParticipants: number;
        tags: string[];
        coverImage?: string;
      };
    },
  ) {
    try {
      return await this.eventService.createEvent(data.userId, data.login, data.input);
    } catch (error: any) {
      this.logger.error('Error creating event:', error);
      return { status: 'error', message: error?.message ?? 'Failed to create event' };
    }
  }

  @MessagePattern({ cmd: 'event_update' })
  async updateEvent(
    @Payload()
    data: {
      eventId: string;
      login: string;
      patch: {
        title?: string;
        description?: string;
        campus?: string;
        date?: string;
        time?: string;
        location?: string;
        maxParticipants?: number;
        tags?: string[];
        coverImage?: string;
      };
    },
  ) {
    try {
      return await this.eventService.updateEvent(data.eventId, data.login, data.patch);
    } catch (error: any) {
      this.logger.error('Error updating event:', error);
      return { status: 'error', message: error?.message ?? 'Failed to update event' };
    }
  }

  @MessagePattern({ cmd: 'event_delete' })
  async deleteEvent(@Payload() data: { eventId: string; login: string }) {
    try {
      return await this.eventService.deleteEvent(data.eventId, data.login);
    } catch (error: any) {
      this.logger.error('Error deleting event:', error);
      return { status: 'error', message: error?.message ?? 'Failed to delete event' };
    }
  }

  @MessagePattern({ cmd: 'event_join' })
  async joinEvent(@Payload() data: { eventId: string; login: string }) {
    try {
      return await this.eventService.joinEvent(data.eventId, data.login);
    } catch (error: any) {
      this.logger.error('Error joining event:', error);
      return { status: 'error', message: error?.message ?? 'Failed to join event' };
    }
  }

  @MessagePattern({ cmd: 'event_leave' })
  async leaveEvent(@Payload() data: { eventId: string; login: string }) {
    try {
      return await this.eventService.leaveEvent(data.eventId, data.login);
    } catch (error: any) {
      this.logger.error('Error leaving event:', error);
      return { status: 'error', message: error?.message ?? 'Failed to leave event' };
    }
  }

  @MessagePattern({ cmd: 'public_api_validate_key' })
  async validateApiKey(@Payload() data: { apiKey: string }) {
    try {
      return await this.publicApiService.validateApiKey(data.apiKey);
    } catch (error: any) {
      this.logger.error('Error validating API key:', error);
      return { status: 'error', message: 'Failed to validate API key' };
    }
  }

  @MessagePattern({ cmd: 'public_resource_list' })
  async listPublicResources(@Payload() data: { limit?: number; offset?: number }) {
    try {
      return await this.publicApiService.listResources(data.limit, data.offset);
    } catch (error: any) {
      this.logger.error('Error listing public resources:', error);
      return { status: 'error', message: 'Failed to list resources' };
    }
  }

  @MessagePattern({ cmd: 'public_resource_get' })
  async getPublicResource(@Payload() data: { id: string }) {
    try {
      return await this.publicApiService.getResource(data.id);
    } catch (error: any) {
      this.logger.error('Error getting public resource:', error);
      return { status: 'error', message: 'Failed to get resource' };
    }
  }

  @MessagePattern({ cmd: 'public_resource_create' })
  async createPublicResource(
    @Payload() data: { title: string; content?: string; tags?: string[] },
  ) {
    try {
      return await this.publicApiService.createResource(data);
    } catch (error: any) {
      this.logger.error('Error creating public resource:', error);
      return { status: 'error', message: error?.message ?? 'Failed to create resource' };
    }
  }

  @MessagePattern({ cmd: 'public_resource_update' })
  async updatePublicResource(
    @Payload()
    data: { id: string; title?: string; content?: string; tags?: string[] },
  ) {
    try {
      return await this.publicApiService.updateResource(data.id, data);
    } catch (error: any) {
      this.logger.error('Error updating public resource:', error);
      return { status: 'error', message: error?.message ?? 'Failed to update resource' };
    }
  }

  @MessagePattern({ cmd: 'public_resource_delete' })
  async deletePublicResource(@Payload() data: { id: string }) {
    try {
      return await this.publicApiService.deleteResource(data.id);
    } catch (error: any) {
      this.logger.error('Error deleting public resource:', error);
      return { status: 'error', message: 'Failed to delete resource' };
    }
  }
}
