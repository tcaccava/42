import type { NextConfig } from "next";

const nextConfig: NextConfig = {
  allowedDevOrigins: ["0.0.0.0", "localhost", "127.0.0.1", process.env.HOST_IP].filter(Boolean) as string[],
};

export default nextConfig;
