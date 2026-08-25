import { useState, useEffect } from 'react';

/**
 * Parses the current pathname into route info.
 * @returns {{ route: 'landing' } | { route: 'game', roomName: string, playerName: string }}
 */
function parsePath() {
  const parts = window.location.pathname.split('/').filter(Boolean);
  if (parts.length >= 2) {
    return { route: 'game', roomName: parts[0], playerName: parts.slice(1).join('/') };
  }
  return { route: 'landing' };
}

/**
 * Programmatic navigation — pushes to history and dispatches popstate.
 * @param {string} path
 */
function navigate(path) {
  window.history.pushState(null, '', path);
  window.dispatchEvent(new PopStateEvent('popstate'));
}

/**
 * React hook that returns the current parsed route.
 * @returns {{ route: 'landing' } | { route: 'game', roomName: string, playerName: string }}
 */
function useRoute() {
  const [route, setRoute] = useState(parsePath);

  useEffect(() => {
    const onPop = () => setRoute(parsePath());
    window.addEventListener('popstate', onPop);
    return () => window.removeEventListener('popstate', onPop);
  }, []);

  return route;
}

export { navigate, useRoute };
