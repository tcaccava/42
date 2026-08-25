import Scanlines from './components/Scanlines';
import Header from './components/Header';
import Footer from './components/Footer';
import LandingPage from './pages/LandingPage';
import GamePage from './pages/GamePage';
import { useRoute } from './router';

function App() {
  const route = useRoute();

  return (
    <div className="h-dvh flex flex-col relative overflow-hidden">
      <Scanlines />
      <Header />
      {route.route === 'landing' ? <LandingPage /> : <GamePage roomName={route.roomName} playerName={route.playerName} />}
      <Footer />
    </div>
  );
}

export default App;
