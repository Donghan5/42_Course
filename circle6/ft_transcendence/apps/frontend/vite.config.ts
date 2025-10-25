import { defineConfig } from 'vite'
import path from 'path'

export default defineConfig({
  root: '.',
  publicDir: 'public',
  build: {
    outDir: 'dist',
    rollupOptions: {
      input: {
        main: './index.html'
      }
    }
  },
  server: {
    port: 3001,
    host: true
  },
  resolve: {
    alias: {
      '@trans/common-types': path.resolve(__dirname, '../../packages/common-types/src')
    }
  }
})
