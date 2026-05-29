import { Image } from 'expo-image';
import { Platform, StyleSheet, Button, View, Text, TextInput, TouchableOpacity, Dimensions, useWindowDimensions } from 'react-native';
import { StatusBar } from 'expo-status-bar';
import { SafeAreaView } from 'react-native-safe-area-context';
import { useState, useEffect } from 'react';

import { HelloWave } from '@/components/hello-wave';
import ParallaxScrollView from '@/components/parallax-scroll-view';
import { ThemedText } from '@/components/themed-text';
import { ThemedView } from '@/components/themed-view';
import { Link } from 'expo-router';

export default function HomeScreen() {
  const [expression, setExpression] = useState('0');
  const [result, setResult] = useState('0');
  const [countNumber, setCountNumber] = useState(0);
  const [numberDigited, setNumberDigited] = useState(false);
  
  // Get screen dimensions for responsive design with orientation support
  const { width, height } = useWindowDimensions();
  const isLandscape = width > height;
  const isTablet = width > 600;

  // Debug: log orientation changes
  useEffect(() => {
    console.log(`Orientation changed: ${isLandscape ? 'Landscape' : 'Portrait'} (${width}x${height})`);
  }, [isLandscape, width, height]);

  const isNumber = (value: string) => !isNaN(Number(value));

  const handleButtonPress = (buttonText: string) => {
    if (buttonText === 'AC') {
      setExpression('0');
      setResult('0');
      setCountNumber(0);
    }
    if (buttonText === 'C') {
      // Clear last character
      if (expression.length > 1) {
        isNumber(expression.slice(0, -1)) ? setCountNumber(countNumber - 1) : null;
        setExpression(expression.slice(0, -1));
      } else {
        setExpression('0');
        setCountNumber(0);
      }
    }
    if (buttonText === '=') {
      try {
        // Evaluate the expression safely
        const evalResult = eval(expression);
        setResult(evalResult.toString());
      } catch (error) {
        setResult('Error');
      }
    }
    stateNumber(buttonText);
  };

  const renderButton = (text: string, style?: any) => (
    <TouchableOpacity
      style={[
        styles.button, 
        isTablet && styles.buttonTablet,
        isLandscape && styles.buttonLandscape,
        style
      ]}
      onPress={() => handleButtonPress(text)}
    >
      <Text style={[
        styles.buttonText,
        isTablet && styles.buttonTextTablet,
        isLandscape && styles.buttonTextLandscape
      ]}>{text}</Text>
    </TouchableOpacity>
  );

  const stateNumber = (buttonText: string) => {
    if (isNumber(buttonText)) {
      // Append digit to expression
      if (countNumber >= 20) return; // Limit to 20 digits
      setCountNumber(countNumber + 1);
      setNumberDigited(true);
      setExpression(prev => prev === '0' ? buttonText : prev + buttonText);
    }
    if (buttonText === '.') {
      // Append decimal point if not already present
      if (!expression.includes('.')) {
        setExpression(prev => prev === '0' ? '0.' : prev + '.');
      }
    }
    if (buttonText === '-' && !numberDigited) {
      // Append negative sign if not already present
      if (!expression.startsWith('-')) {
        setExpression(prev => prev === '0' ? '-' : '-');
      }
    }
    stateSymbol(buttonText);
  };

  const stateSymbol = (buttonText: string) => {
    if (['+', '-', '*', '/'].includes(buttonText)) {      
      // Append operator if last character is a number
      if (isNumber(expression.slice(-1))) {
        setCountNumber(0);
        setExpression(prev => prev + buttonText);
      }
    }
  };

  return (
    <SafeAreaView style={[styles.safeContainer, isLandscape && styles.safeContainerLandscape]}>
      <StatusBar style="light" />
      
      {/* Custom AppBar */}
      <View style={[styles.appBar, isLandscape && styles.appBarLandscape]}>
        <Text style={styles.appBarTitle}>Calculator</Text>
      </View>
      
      {/* Calculator Content */}
      <View style={[
        styles.calculatorContainer,
        isLandscape && styles.calculatorContainerLandscape
      ]}>
        {/* Display Area */}
        <View style={[
          styles.displayContainer,
          isLandscape && styles.displayContainerLandscape
        ]}>
          {/* Expression TextField */}
          <TextInput
            style={[
              styles.expressionInput,
              isLandscape && styles.expressionInputLandscape
            ]}
            value={expression}
            editable={false}
            multiline={false}
          />
          
          {/* Result TextField */}
          <TextInput
            style={[
              styles.resultInput,
              isLandscape && styles.resultInputLandscape
            ]}
            value={result}
            editable={false}
            multiline={false}
          />
        </View>
        
        {/* Buttons Area */}
        <View style={[
          styles.buttonContainer,
          isLandscape && styles.buttonContainerLandscape
        ]}>
          {/* Row 1: AC, C, /, * */}
          <View style={styles.buttonRow}>
            {renderButton('AC', styles.operatorButton)}
            {renderButton('C', styles.operatorButton)}
            {renderButton('/', styles.operatorButton)}
            {renderButton('*', styles.operatorButton)}
          </View>
          
          {/* Row 2: 7, 8, 9, - */}
          <View style={styles.buttonRow}>
            {renderButton('7')}
            {renderButton('8')}
            {renderButton('9')}
            {renderButton('-', styles.operatorButton)}
          </View>
          
          {/* Row 3: 4, 5, 6, + */}
          <View style={styles.buttonRow}>
            {renderButton('4')}
            {renderButton('5')}
            {renderButton('6')}
            {renderButton('+', styles.operatorButton)}
          </View>
          
          {/* Row 4: 1, 2, 3, = */}
          <View style={styles.buttonRow}>
            {renderButton('1')}
            {renderButton('2')}
            {renderButton('3')}
            <TouchableOpacity
              style={[
                styles.button, 
                styles.equalsButton,
                isTablet && styles.buttonTablet,
                isTablet && styles.equalsButtonTablet,
                isLandscape && styles.buttonLandscape,
                isLandscape && styles.equalsButtonLandscape
              ]}
              onPress={() => handleButtonPress('=')}
            >
              <Text style={[
                styles.buttonText,
                isTablet && styles.buttonTextTablet,
                isLandscape && styles.buttonTextLandscape
              ]}>=</Text>
            </TouchableOpacity>
          </View>

          
          
          {/* Row 5: 0, . */}
          <View style={[styles.buttonRowSpecial, isLandscape && styles.buttonRowSpecialLandscape]}>
            <TouchableOpacity
              style={[
                styles.button, 
                styles.zeroButton,
                isTablet && styles.buttonTablet,
                isTablet && styles.zeroButtonTablet,
                isLandscape && styles.buttonLandscape,
                isLandscape && styles.zeroButtonLandscape
              ]}
              onPress={() => handleButtonPress('0')}
            >
              <Text style={[
                styles.buttonText,
                isTablet && styles.buttonTextTablet,
                isLandscape && styles.buttonTextLandscape
              ]}>0</Text>
            </TouchableOpacity>
            {renderButton('.')}
            <View
              pointerEvents="none"
              style={[
                styles.button,
                isTablet && styles.buttonTablet,
                isLandscape && styles.buttonLandscape,
                styles.placeholderButton
              ]}
            />
          </View>
        </View>
      </View>
    </SafeAreaView>
  );
}

const styles = StyleSheet.create({
  safeContainer: {
    flex: 1,
    backgroundColor: '#2196F3',
  },
  safeContainerLandscape: {
    backgroundColor: '#000',
  },
  appBar: {
    height: 36,
    backgroundColor: '#2196F3',
    justifyContent: 'center',
    alignItems: 'center',
    elevation: 4,
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 3.84,
  },
  appBarLandscape: {
    backgroundColor: '#000',
    elevation: 0,
    shadowOpacity: 0,
  },
  appBarTitle: {
    color: 'white',
    fontSize: 20,
    fontWeight: 'bold',
  },
  calculatorContainer: {
    flex: 1,
    backgroundColor: '#000',
    paddingHorizontal: 20,
    paddingVertical: 10,
  },
  calculatorContainerLandscape: {
    flexDirection: 'column', // Mantieni layout verticale
    paddingHorizontal: 40,   // Più padding sui lati
    paddingVertical: 5,
    alignItems: 'center',    // Centra tutto
  },
  displayContainer: {
    flex: 0.6,
    justifyContent: 'flex-end',
    paddingVertical: 15,
  },
  displayContainerLandscape: {
    flex: 0.15,              // Spazio molto ridotto per il display
    paddingVertical: 5,
    width: '100%',           // Larghezza completa
    maxWidth: 600,           // Limite massimo
  },
  expressionInput: {
    fontSize: 20,
    color: '#FFF',
    textAlign: 'right',
    backgroundColor: 'transparent',
    marginBottom: 8,
    paddingHorizontal: 10,
    paddingVertical: 3,
  },
  expressionInputLandscape: {
    fontSize: 16,
    marginBottom: 5,
  },
  resultInput: {
    fontSize: 36,
    color: '#FFF',
    textAlign: 'right',
    backgroundColor: 'transparent',
    paddingHorizontal: 10,
    paddingVertical: 3,
    fontWeight: 'bold',
  },
  resultInputLandscape: {
    fontSize: 28,
  },
  buttonContainer: {
    flex: 1,
    justifyContent: 'space-around',
    paddingBottom: 15,
  },
  buttonContainerLandscape: {
    flex: 0.85,
    paddingBottom: 8,
    width: '100%',
    maxWidth: 520,
    alignSelf: 'center',
  },
  buttonRow: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginVertical: 3,
  },
  buttonRowSpecial: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginTop: '-20%',
  },
  buttonRowSpecialLandscape: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginTop: '-10%',
  },
  button: {
    backgroundColor: '#333',
    borderRadius: 35,
    justifyContent: 'center',
    alignItems: 'center',
    width: 70,
    height: 70,
  },
  buttonLandscape: {
    width: 48,
    height: 48,
    borderRadius: 24,
  },
  buttonText: {
    color: '#FFF',
    fontSize: 20,
    fontWeight: 'bold',
  },
  buttonTextLandscape: {
    fontSize: 18,            // Testo leggermente più piccolo
  },
  operatorButton: {
    backgroundColor: '#FF9500',
  },
  placeholderButton: {
    opacity: 0,
  },
  equalsButton: {
    backgroundColor: '#FF9500',
    height: 145,
    borderRadius: 35,
  },
  equalsButtonLandscape: {
    height: 106, // roughly 48 * 2 + spacing allowance
    borderRadius: 24,
  },
  zeroButton: {
    width: 145,
    borderRadius: 35,
  },
  zeroButtonLandscape: {
    width: 106, // roughly 48 * 2 + spacing allowance
    borderRadius: 24,
  },
  // Tablet responsive styles
  buttonTablet: {
    width: 90,
    height: 90,
    borderRadius: 45,
  },
  buttonTextTablet: {
    fontSize: 28,
  },
  zeroButtonTablet: {
    width: 185,
  },
  equalsButtonTablet: {
    height: 185,
  },
});