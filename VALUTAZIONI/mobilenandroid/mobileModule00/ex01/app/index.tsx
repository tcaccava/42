import { Image } from 'expo-image';
import { Platform, StyleSheet, Button } from 'react-native';

import { HelloWave } from '@/components/hello-wave';
import ParallaxScrollView from '@/components/parallax-scroll-view';
import { ThemedText } from '@/components/themed-text';
import { ThemedView } from '@/components/themed-view';
import { Link } from 'expo-router';
import { useState } from 'react';

export default function HomeScreen() {

  const [buttonPressed, setButtonPressed] = useState(false);

  const handleButtonPress = () => {
    setButtonPressed(buttonPressed => !buttonPressed);
  };

  return (
    <ThemedView style={styles.container}>
      <ThemedView style={styles.titleContainer}>
        <ThemedText>{buttonPressed ? 'Hello World' : 'Sample Text'}</ThemedText>
      </ThemedView>
      <Button title="Click Me" onPress={handleButtonPress} />
    </ThemedView>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  titleContainer: {
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'center',
  }
});