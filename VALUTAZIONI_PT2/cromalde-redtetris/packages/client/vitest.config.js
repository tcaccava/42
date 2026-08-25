import { defineConfig } from 'vitest/config';

export default defineConfig({
  test: {
    environment: 'jsdom',
    setupFiles: ['./src/test-setup.js'],
    coverage: {
      statements: 70,
      functions: 70,
      lines: 70,
      branches: 50,
      include: ['src/**/*.{js,jsx}'],
    },
  },
});
