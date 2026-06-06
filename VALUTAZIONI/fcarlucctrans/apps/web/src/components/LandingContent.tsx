'use client';

import { LoginButton } from '@/components/LoginButton';
import { Code2, Users, Calendar, ArrowRight, Github, Terminal, Heart, GitFork } from 'lucide-react';
import Image from 'next/image';
import { LegalLinks } from '@/components/legal/LegalLinks';
import { local } from '@/utilities';

export default function LandingContent() {
  const t = local.useLocalization().t;
  return (
    <div className="min-h-screen bg-black text-white">
      {/* Simplified Background - solo verde */}
      <div className="fixed inset-0 overflow-hidden pointer-events-none">
        <div className="absolute inset-0 bg-gradient-to-b from-green-950/20 via-black to-black"></div>
        <div className="absolute top-0 right-1/4 w-96 h-96 bg-green-500 rounded-full mix-blend-multiply filter blur-3xl opacity-5 animate-pulse"></div>
      </div>

      {/* Hero Section */}
      <div className="relative">
        <div className="max-w-7xl mx-auto px-4 pt-20 pb-32 sm:pt-32 sm:pb-48">
          {/* Logo + Brand */}
          <div className="flex items-center justify-center gap-3 mb-8">
            <Image
              src="/42logo.png"
              alt="42 Logo"
              width={24}
              height={24}
              className="w-12 h-12 object-contain brightness-0 invert"
            />
            <h1 className="text-3xl sm:text-4xl font-black tracking-tight">
              <span className="text-[#05BBBB]">{'<'}</span>
              <span className="text-white">Share</span>
              <span className="text-green-500">{'/'}</span>
              <span className="text-[#05BBBB]">{'>'}</span>
            </h1>
          </div>

          {/* Open Source Badge */}
          <div className="flex items-center justify-center gap-3 mb-12">
            <a
              href="https://github.com"
              target="_blank"
              rel="noopener noreferrer"
              className="inline-flex items-center gap-2 px-4 py-2 rounded-full border border-green-500/30 bg-green-500/5 hover:bg-green-500/10 transition-colors"
            >
              <Github className="w-4 h-4 text-green-500" />
              <span className="text-sm font-mono text-green-400">Open Source</span>
            </a>
            <div className="inline-flex items-center gap-2 px-4 py-2 rounded-full border border-gray-700 bg-gray-900/50">
              <Heart className="w-4 h-4 text-gray-400" />
              <span className="text-sm text-gray-400">{t.builtCommunity}</span>
            </div>
          </div>

          {/* Main Headline */}
          <div className="text-center max-w-5xl mx-auto">
            <h2 className="text-5xl sm:text-7xl lg:text-8xl font-black mb-8 leading-none">
              {t.shareCode}.
              <br />
              <span className="text-green-500">{t.buildTogether}.</span>
            </h2>

            <p className="text-xl sm:text-2xl text-gray-400 mb-12 max-w-2xl mx-auto font-light">
              {t.communityDriven}.
              <br />
              <span className="text-green-500 font-mono">Open source</span> {t.acceptingContribution}.
            </p>

            {/* CTA */}
            <div className="flex flex-col sm:flex-row items-center justify-center gap-4 mb-20">
              <LoginButton />
              <a
                href="https://github.com"
                target="_blank"
                rel="noopener noreferrer"
                aria-label="Contribute on GitHub (opens in new tab)"
                className="group px-8 py-4 border border-gray-700 text-white font-semibold text-lg rounded-xl hover:border-green-500 transition-all duration-300 flex items-center gap-2"
              >
                <GitFork className="w-5 h-5" />
                {t.contribute}
              </a>
            </div>

            {/* Live Stats - palette pulita */}
            <div className="grid grid-cols-3 gap-4 max-w-3xl mx-auto">
              <div className="bg-gray-900 border border-gray-800 rounded-2xl p-6 hover:border-green-500/50 transition-colors">
                <div className="text-4xl font-black text-green-500 mb-1 font-mono">5K+</div>
                <div className="text-gray-400 text-sm">{t.projects}</div>
              </div>
              <div className="bg-gray-900 border border-gray-800 rounded-2xl p-6 hover:border-green-500/50 transition-colors">
                <div className="text-4xl font-black text-white mb-1 font-mono">200+</div>
                <div className="text-gray-400 text-sm">{t.events}</div>
              </div>
              <div className="bg-gray-900 border border-gray-800 rounded-2xl p-6 hover:border-green-500/50 transition-colors">
                <div className="text-4xl font-black text-white mb-1 font-mono">1K+</div>
                <div className="text-gray-400 text-sm">{t.students}</div>
              </div>
            </div>
          </div>
        </div>
      </div>

      {/* Bento Grid Features */}
      <div id="features" className="relative py-20 bg-gray-950">
        <div className="max-w-7xl mx-auto px-4">
          <div className="text-center mb-16">
            <h3 className="text-4xl sm:text-5xl font-black text-white mb-4">
              {t.builtCommunity},
              <br />
              <span className="text-green-500">{t.forCommunity}</span>
            </h3>
            <p className="text-xl text-gray-400">
              {t.openSource}
            </p>
          </div>

          {/* Bento Layout - palette pulita */}
          <div className="grid md:grid-cols-2 lg:grid-cols-3 gap-4">
            {/* Large Feature - Open Source */}
            <div className="lg:col-span-2 lg:row-span-2 relative group overflow-hidden rounded-3xl bg-gray-900 border border-gray-800 p-8 hover:border-green-500/50 transition-all">
              <Github className="w-12 h-12 text-green-500 mb-6" />
              <h4 className="text-3xl font-bold text-white mb-4">100% Open Source</h4>
              <p className="text-gray-400 text-lg mb-6 max-w-md">
                {t.publicCode}<span className="text-red-500">❤</span>
              </p>
              <div className="bg-black rounded-xl p-4 font-mono text-sm border border-gray-800">
                <div className="text-gray-500"># {t.cloneContribute}</div>
                <div className="text-white">$ git clone github.com/42share</div>
                <div className="text-gray-500"># {t.installDependencies}</div>
                <div className="text-white">$ npm install</div>
                <div className="text-green-500">✓ {t.readyHack}</div>
              </div>
            </div>

            {/* Small Features - palette semplice */}
            <div className="relative overflow-hidden rounded-3xl bg-gray-900 border border-gray-800 p-6 hover:border-green-500/50 transition-all">
              <Terminal className="w-10 h-10 text-green-500 mb-4" />
              <h4 className="text-xl font-bold text-white mb-2">{t.shareProjects}</h4>
              <p className="text-gray-400 text-sm">
                {t.showcaseProjects}
              </p>
            </div>

            <div className="relative overflow-hidden rounded-3xl bg-gray-900 border border-gray-800 p-6 hover:border-green-500/50 transition-all">
              <Users className="w-10 h-10 text-white mb-4" />
              <h4 className="text-xl font-bold text-white mb-2">{t.globalNetwork}</h4>
              <p className="text-gray-400 text-sm">
                {t.connectStudents}
              </p>
            </div>

            <div className="relative overflow-hidden rounded-3xl bg-gray-900 border border-gray-800 p-6 hover:border-green-500/50 transition-all">
              <Calendar className="w-10 h-10 text-white mb-4" />
              <h4 className="text-xl font-bold text-white mb-2">{ t.campusEvents }</h4>
              <p className="text-gray-400 text-sm">
                {t.campusEventsDesc}
              </p>
            </div>

            <div className="relative overflow-hidden rounded-3xl bg-gray-900 border border-gray-800 p-6 hover:border-green-500/50 transition-all">
              <Code2 className="w-10 h-10 text-white mb-4" />
              <h4 className="text-xl font-bold text-white mb-2">{t.portfolioReady}</h4>
              <p className="text-gray-400 text-sm">
                {t.portfolioReadyDesc}
              </p>
            </div>
          </div>
        </div>
      </div>

      {/* CTA Section - semplificato */}
      <div className="relative py-32 overflow-hidden bg-black">
        <div className="relative max-w-4xl mx-auto px-4 text-center">
          <GitFork className="w-16 h-16 mx-auto mb-6 text-green-500" />
          <h3 className="text-4xl sm:text-6xl font-black text-white mb-6">
            {t.readyTo}<span className="text-green-500">{t.contribute}</span>?
          </h3>
          <p className="text-xl text-gray-400 mb-10">
            {t.joinCommunity}
          </p>
          <LoginButton />
          <p className="text-sm text-gray-600 mt-6">
            {t.intraOAuth}
          </p>
        </div>
      </div>

      {/* Footer */}
      <div className="relative z-10 border-t border-gray-700 py-12 bg-gray-950">
        <div className="max-w-7xl mx-auto px-4">
          <div className="flex flex-col md:flex-row justify-between items-center gap-6">
            <div className="flex items-center gap-2">
              <Image
                src="/42logo.png"
                alt="42 Logo"
                width={24}
                height={24}
                className="w-10 h-10 object-contain brightness-0 invert"
              />
              <h1 className="text-2xl sm:text-3xl font-black tracking-tight">
                <span className="text-[#05BBBB]">{'<'}</span>
                <span className="text-white">Share</span>
                <span className="text-green-500">{'/'}</span>
                <span className="text-[#05BBBB]">{'>'}</span>
              </h1>
            </div>
            <div className="flex items-center gap-6">
              <a
                href="https://github.com"
                target="_blank"
                rel="noopener noreferrer"
                className="flex items-center gap-2 text-gray-500 hover:text-green-500 transition-colors text-sm"
              >
                <Github className="w-4 h-4" />
                <span>Open Source</span>
              </a>
              <a
                href="https://github.com"
                target="_blank"
                rel="noopener noreferrer"
                className="flex items-center gap-2 text-gray-500 hover:text-green-500 transition-colors text-sm"
              >
                <GitFork className="w-4 h-4" />
                <span>{t.contribute}</span>
              </a>
            </div>
            <div className="flex flex-col items-center gap-3 text-sm text-gray-600 md:items-end">
              <LegalLinks linkClassName="text-gray-500 transition-colors hover:text-green-500" />
              <span>© 2026 • {t.builtCommunity}</span>
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
