import { defineConfig } from 'vitest/config';

export default defineConfig({
  test: {
    environment: 'node',
    coverage: {
      statements: 70,
      functions: 70,
      lines: 70,
      branches: 50,
    },
  },
});
