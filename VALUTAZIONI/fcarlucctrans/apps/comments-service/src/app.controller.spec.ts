import { Test, TestingModule } from '@nestjs/testing';
import { CommentController } from './comment.controller';
import { CommentService } from './comment.service';

describe('AppController', () => {
	let appController: CommentController;

	beforeEach(async () => {
		const app: TestingModule = await Test.createTestingModule({
			controllers: [CommentController],
			providers: [CommentService],
		}).compile();

		appController = app.get<CommentController>(CommentController);
	});

	describe('root', () => {
		it('should return "Hello World!"', () => {
			expect(appController.getHello()).toBe('Hello World!');
		});
	});
});
