import os
import sys
import django

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "config.settings")
django.setup()

from apps.gameplay.models import Question

questions = [
    {
        "text": "Which planet is known as the Red Planet?",
        "a": "Earth",
        "b": "Mars",
        "c": "Jupiter",
        "d": "Venus",
        "answer": "Mars"
    },
    {
        "text": "Who wrote 'Romeo and Juliet'?",
        "a": "Charles Dickens",
        "b": "William Shakespeare",
        "c": "Jane Austen",
        "d": "Mark Twain",
        "answer": "William Shakespeare"
    },
    {
        "text": "What is the largest ocean on Earth?",
        "a": "Atlantic Ocean",
        "b": "Indian Ocean",
        "c": "Arctic Ocean",
        "d": "Pacific Ocean",
        "answer": "Pacific Ocean"
    },
    {
        "text": "What is the chemical formula for ice?",
        "a": "O2",
        "b": "CO2",
        "c": "H2O",
        "d": "NaCl",
        "answer": "H2O"
    },
    {
        "text": "In which year did the Titanic sink?",
        "a": "1898",
        "b": "1909",
        "c": "1912",
        "d": "1921",
        "answer": "1912"
    },
    {
        "text": "What is the hardest natural substance on Earth?",
        "a": "Gold",
        "b": "Iron",
        "c": "Diamond",
        "d": "Platinum",
        "answer": "Diamond"
    },
    {
        "text": "Who painted the Mona Lisa?",
        "a": "Vincent van Gogh",
        "b": "Pablo Picasso",
        "c": "Leonardo da Vinci",
        "d": "Michelangelo",
        "answer": "Leonardo da Vinci"
    },
    {
        "text": "What is the smallest prime number?",
        "a": "0",
        "b": "1",
        "c": "2",
        "d": "3",
        "answer": "2"
    },
    {
        "text": "Which country is home to the kangaroo?",
        "a": "South Africa",
        "b": "New Zealand",
        "c": "Brazil",
        "d": "Australia",
        "answer": "Australia"
    },
    {
        "text": "What is the main ingredient in guacamole?",
        "a": "Tomato",
        "b": "Avocado",
        "c": "Onion",
        "d": "Pepper",
        "answer": "Avocado"
    },
    {
        "text": "How many strings does a standard viola have?",
        "a": "3",
        "b": "4",
        "c": "5",
        "d": "6",
        "answer": "4"
    },
    {
        "text": "Who was the first person to walk on the moon?",
        "a": "Louis Armstrong",
        "b": "Yuri Gagarin",
        "c": "Buzz Aldrin",
        "d": "Neil Armstrong",
        "answer": "Neil Armstrong"
    },
    {
        "text": "Which famous scientist developed the theory of relativity?",
        "a": "Isaac Newton",
        "b": "Galileo Galilei",
        "c": "Albert Einstein",
        "d": "Nikola Tesla",
        "answer": "Albert Einstein"
    },
    {
        "text": "What is the currency of China?",
        "a": "Won",
        "b": "Renminbi",
        "c": "Yen",
        "d": "Ringgit",
        "answer": "Renminbi"
    },
    {
        "text": "How many bones are in the adult human body?",
        "a": "206",
        "b": "198",
        "c": "212",
        "d": "189",
        "answer": "206"
    },
    {
        "text": "What is the tallest mountain in the world?",
        "a": "K2",
        "b": "Kangchenjunga",
        "c": "Mount Everest",
        "d": "Lhotse",
        "answer": "Mount Everest"
    },
    {
        "text": "In which country was Albert Einstein born?",
        "a": "Switzerland",
        "b": "Austria",
        "c": "Germany",
        "d": "United States",
        "answer": "Germany"
    },
    {
        "text": "What is the largest continent by land area?",
        "a": "Africa",
        "b": "North America",
        "c": "Asia",
        "d": "Europe",
        "answer": "Asia"
    },
    {
        "text": "Which element has the chemical symbol 'O'?",
        "a": "Gold",
        "b": "Oganesson",
        "c": "Osmium",
        "d": "Oxygen",
        "answer": "Oxygen"
    },
    {
        "text": "Who is the author of the 'Harry Potter' series?",
        "a": "J.R.R. Tolkien",
        "b": "Roald Dahl",
        "c": "C.S. Lewis",
        "d": "J.K. Rowling",
        "answer": "J.K. Rowling"
    },
    {
        "text": "What is the fastest land animal?",
        "a": "Lion",
        "b": "Pronghorn",
        "c": "Cheetah",
        "d": "Quarter Horse",
        "answer": "Cheetah"
    },
    {
        "text": "In which year was the Colosseum in Rome completed?",
        "a": "124 BC",
        "b": "7 BC",
        "c": "80 AD",
        "d": "175 AD",
        "answer": "80 AD"
    },

    {
        "text": "What does 'CPU' stand for?",
        "a": "Central Processing Unit",
        "b": "Computer Personal Unit",
        "c": "Central Program Utility",
        "d": "Core Performance Unit",
        "answer": "Central Processing Unit"
    },
    {
        "text": "Which programming language is primarily used for Android app development?",
        "a": "Swift",
        "b": "Python",
        "c": "Java",
        "d": "C#",
        "answer": "Java"
    },
    {
        "text": "What does HTML stand for?",
        "a": "Hyper Trainer Markup Language",
        "b": "Hyper Text Markup Language",
        "c": "High Tech Multi Language",
        "d": "Hyper Transfer Markup Language",
        "answer": "Hyper Text Markup Language"
    },
    {
        "text": "Which of these is an example of an operating system?",
        "a": "Microsoft Word",
        "b": "Google Chrome",
        "c": "Linux",
        "d": "MySQL",
        "answer": "Linux"
    },
    {
        "text": "What is the name of the first electronic general-purpose computer?",
        "a": "UNIVAC",
        "b": "ENIAC",
        "c": "IBM 701",
        "d": "Colossus",
        "answer": "ENIAC"
    },
    {
        "text": "What is the derivative of x^2?",
        "a": "x",
        "b": "2x",
        "c": "x^2",
        "d": "2",
        "answer": "2x"
    },
    {
        "text": "What is the value of 2^5?",
        "a": "10",
        "b": "25",
        "c": "32",
        "d": "64",
        "answer": "32"
    },
    {
        "text": "What is the next prime number after 29?",
        "a": "31",
        "b": "33",
        "c": "35",
        "d": "37",
        "answer": "31"
    },
    {
        "text": "Which actor played the Joker in The Dark Knight (2008)?",
        "a": "Jared Leto",
        "b": "Joaquin Phoenix",
        "c": "Heath Ledger",
        "d": "Jack Nicholson",
        "answer": "Heath Ledger"
    },
    {
        "text": "Which director created the movie Inception?",
        "a": "Steven Spielberg",
        "b": "James Cameron",
        "c": "Ridley Scott",
        "d": "Christopher Nolan",
        "answer": "Christopher Nolan"
    },
    {
        "text": "Which band released the album 'Dark Side of the Moon'?",
        "a": "Led Zeppelin",
        "b": "Queen",
        "c": "The Beatles",
        "d": "Pink Floyd",
        "answer": "Pink Floyd"
    },
    {
        "text": "In the Harry Potter series, what is the name of Voldemort's snake?",
        "a": "Nagini",
        "b": "Aragog",
        "c": "Fang",
        "d": "Basilisk",
        "answer": "Nagini"
    },
    {
        "text": "Which video game company created the Sonic series?",
        "a": "Sony",
        "b": "Microsoft",
        "c": "Nintendo",
        "d": "Sega",
        "answer": "Sega"
    },
    {
        "text": "Which singer is known as the 'King of Pop'?",
        "a": "Elvis Presley",
        "b": "Freddie Mercury",
        "c": "Prince",
        "d": "Michael Jackson",
        "answer": "Michael Jackson"
    },
    {
        "text": "On an American One Hundred Dollar Bill, there is a portrait of which American statesman?",
        "a": "George Washington",
        "b": "Franklin Roosevelt",
        "c": "Benjamin Franklin",
        "d": "Abraham Lincoln",
        "answer": "Benjamin Franklin"
    },
    {
        "text": "Who invented the revolver?",
        "a": "Samuel Colt",
        "b": "Bruce Browning",
        "c": "Dan Wesson",
        "d": "James Revolver",
        "answer": "Samuel Colt"
    },
    {
        "text": "In Alexander Dumas' book, \"The Three Musketeers\", two of the musketeers are called Athos and Porthos. What was the name of the third Musketeer?",
        "a": "Aramis",
        "b": "Cardinal Richelieu",
        "c": "D'Artagnan",
        "d": "Planchet",
        "answer": "Aramis"
    },
    {
        "text": "Playwright Anton Chekhov graduated from the University of Moscow with a degree in what?",
        "a": "Law",
        "b": "Medicine",
        "c": "Philosophy",
        "d": "Economics",
        "answer": "Medicine"
    },
    {
        "text": "What did Gene Cernan, the last person to walk on the Moon, write on the lunar surface before leaving?",
        "a": "The peace symbol",
        "b": "The initials of his daughter",
        "c": "God bless America",
        "d": "Gene was here",
        "answer": "The initials of his daughter"
    },
    {
        "text": "In which continent is the Kalahari Desert located?",
        "a": "Africa",
        "b": "Oceania",
        "c": "America",
        "d": "Asia",
        "answer": "Africa"
    },
    {
        "text": "Into what does the protagonist of Kafka's The Metamorphosis transform?",
        "a": "An insect",
        "b": "A statue",
        "c": "A fish",
        "d": "A mouse",
        "answer": "An insect"
    },
    {
        "text": "Which of these countries drives on the right?",
        "a": "Japan",
        "b": "Australia",
        "c": "China",
        "d": "India",
        "answer": "China"
    },
    {
        "text": "Who wrote the letter \"J'accuse\" to the President of the French Republic, Félix Faure?",
        "a": "Émile Zola",
        "b": "Victor Hugo",
        "c": "Voltaire",
        "d": "Rousseau",
        "answer": "Émile Zola"
    },
    {
        "text": "Where is the cardia located?",
        "a": "In the kidney",
        "b": "In the lungs",
        "c": "In the stomach",
        "d": "In the heart",
        "answer": "In the stomach"
    },
    {
        "text": "Where is Ernő Rubik, the creator of the Rubik's Cube, from?",
        "a": "Spain",
        "b": "Czech Republic",
        "c": "Germany",
        "d": "Hungary",
        "answer": "Hungary"
    },
    {
        "text": "How many symphonies did Ludwig van Beethoven compose?",
        "a": "5",
        "b": "7",
        "c": "9",
        "d": "12",
        "answer": "9"
    },
    {
        "text": "Which famous Dutch artist created the lithograph 'Drawing Hands', depicting a hand drawing another hand?",
        "a": "Pablo Picasso",
        "b": "Salvador Dalí",
        "c": "Vincent van Gogh",
        "d": "M. C. Escher",
        "answer": "M. C. Escher"
    },
    {
        "text": "Which director holds the record for the most Academy Awards won for Best Director, with 4 wins?",
        "a": "John Ford",
        "b": "Frank Capra",
        "c": "Steven Spielberg",
        "d": "Clint Eastwood",
        "answer": "John Ford"
    },
    {
        "text": "Which artist sang the famous lyrics 'Ground Control to Major Tom, Take your protein pills and put your helmet on'?",
        "a": "Elton John",
        "b": "David Bowie",
        "c": "Freddie Mercury",
        "d": "Paul McCartney",
        "answer": "David Bowie"
    },
    {
        "text": "Which Russian painter is considered one of the pioneers of abstract art and created the famous work 'Composition VIII'?",
        "a": "Kazimir Malevich",
        "b": "Marc Chagall",
        "c": "Wassily Kandinsky",
        "d": "Piet Mondrian",
        "answer": "Wassily Kandinsky"
    },
    {
        "text": "The Great Wall of China stretches for about 21,196 km. What is the average height of the wall?",
        "a": "About 3 meters",
        "b": "About 8 meters",
        "c": "About 15 meters",
        "d": "About 25 meters",
        "answer": "About 8 meters"
    },
    {
        "text": "Which country has the most UNESCO World Heritage Sites?",
        "a": "China",
        "b": "Italy",
        "c": "Spain",
        "d": "France",
        "answer": "Italy"
    },
    {
        "text": "Immanuel Kant proposed a theory about the formation of the Solar System. Which scientist’s name is commonly associated with Kant in this theory?",
        "a": "Laplace",
        "b": "Lavoisier",
        "c": "Faraday",
        "d": "Gauss",
        "answer": "Laplace"
    },
    {
        "text": "From which country was the astronomer and physicist Anders Celsius, who created the Celsius temperature scale?",
        "a": "Denmark",
        "b": "Norway",
        "c": "Sweden",
        "d": "Finland",
        "answer": "Sweden"
    },
    {
        "text": "Which Swedish pop group became internationally famous in the 1970s with hits like 'Dancing Queen'?",
        "a": "Roxette",
        "b": "Ace of Base",
        "c": "ABBA",
        "d": "The Cardigans",
        "answer": "ABBA"
    },
    {
        "text": "Which traditional Swedish celebration takes place in June and is known for dancing around a decorated pole called a maypole?",
        "a": "Lucia Day",
        "b": "Walpurgis Night",
        "c": "Midsummer",
        "d": "Crayfish Party",
        "answer": "Midsummer"
    },
    {
        "text": "Nobel Prizes are awarded in Stockholm, Sweden, except for one which is presented in Oslo, Norway. Which one?",
        "a": "Nobel Prize in Literature",
        "b": "Nobel Prize in Economics",
        "c": "Nobel Prize in Medicine",
        "d": "Nobel Peace Prize",
        "answer": "Nobel Peace Prize"
    },
    {
        "text": "Which Swedish pole vaulter, nicknamed 'Mondo', has broken the men's pole vault world record multiple times?",
        "a": "Armand Duplantis",
        "b": "Renaud Lavillenie",
        "c": "Thiago Braz",
        "d": "Sam Kendricks",
        "answer": "Armand Duplantis"
    },
    {
        "text": "In which city were the first modern Olympic Games held in 1896?",
        "a": "Paris",
        "b": "Athens",
        "c": "London",
        "d": "Rome",
        "answer": "Athens"
    },
    {
        "text": "What unusual time-traveling vehicle does the character Doctor Who use in his adventures?",
        "a": "A spaceship called the Millennium Falcon",
        "b": "A magical train",
        "c": "A time machine shaped like a car named Delorean",
        "d": "A blue police box called the TARDIS",
        "answer": "A blue police box called the TARDIS"
    },
    {
        "text": "Construction of which of these famous landmarks was completed first?",
        "a": "Empire State Building",
        "b": "Sagrada Família",
        "c": "Eiffel Tower",
        "d": "'Big Ben' Clock Tower",
        "answer": "'Big Ben' Clock Tower"
    },
    {
        "text": "Which 1997 film earned Robin Williams an Academy Award for Best Supporting Actor?",
        "a": "Dead Poets Society",
        "b": "Good Morning, Vietnam",
        "c": "Will Hunting",
        "d": "Hook",
        "answer": "Will Hunting"
    },
    {
        "text": "What does the Latin term 'alibi' mean?",
        "a": "Elsewhere",
        "b": "Guilty",
        "c": "Evidence",
        "d": "Innocent",
        "answer": "Elsewhere"
    },
    {
        "text": "The atmosphere of Mars is composed primarily of which gas?",
        "a": "Helium",
        "b": "Nitrogen",
        "c": "Carbon dioxide",
        "d": "Methane",
        "answer": "Carbon dioxide"
    },
    {
        "text": "Which of these is a cheese from the Netherlands?",
        "a": "Pecorino",
        "b": "Roquefort",
        "c": "Manchego",
        "d": "Edam",
        "answer": "Edam"
    },
    {
        "text": "Which of these sporting events was the first to be televised live?",
        "a": "Wimbledon tennis tournament",
        "b": "FIFA World Cup",
        "c": "Tour de France",
        "d": "Olympic Games",
        "answer": "Olympic Games"
    },
    {
        "text": "Which of these is a light, porous volcanic rock commonly used to remove rough skin?",
        "a": "Pumice",
        "b": "Basalt",
        "c": "Flint",
        "d": "Anthracite",
        "answer": "Pumice"
    },
    {
        "text": "On a standar dice, what number is opposite the number six?",
        "a": "One",
        "b": "Two",
        "c": "Three",
        "d": "Four",
        "answer": "One"
    },
    {
        "text": "Which 20th-century author wrote the novel 'The Trial' (Der Prozess), a work centered on the character Josef K.?",
        "a": "Franz Kafka",
        "b": "Thomas Mann",
        "c": "Hermann Hesse",
        "d": "Albert Camus",
        "answer": "Franz Kafka"
    },
    {
        "text": "Which English poet wrote the epic poem 'Paradise Lost', first published in 1667?",
        "a": "John Donne",
        "b": "William Blake",
        "c": "John Milton",
        "d": "Alexander Pope",
        "answer": "John Milton"
    },
    {
        "text": "Which of the following works was NOT written by Jules Verne?",
        "a": "Twenty Thousand Leagues Under the Sea",
        "b": "Around the World in Eighty Days",
        "c": "The Time Machine",
        "d": "Journey to the Center of the Earth",
        "answer": "The Time Machine"
    },
    {
        "text": "Which of these is a unit of length, used to measure astronomical distance?",
        "a": "Telsa",
        "b": "Newton",
        "c": "Parsec",
        "d": "Mole",
        "answer": "Parsec"
    },
    {
        "text": "Which of these abbreviations is used when someone wants something done immidiately?",
        "a": "ASAP",
        "b": "YMCA",
        "c": "BYOB",
        "d": "HMRC",
        "answer": "ASAP"
    },
    {
        "text": "The landmark Alhambra place overlooks which Spanish city?",
        "a": "Seville",
        "b": "Málaga",
        "c": "Bilbao",
        "d": "Granada",
        "answer": "Granada"
    },
    {
        "text": "Through which of the following countries does the Danube NOT flow?",
        "a": "Serbia",
        "b": "Germany",
        "c": "Romania",
        "d": "Poland",
        "answer": "Poland"
    },
    {
        "text": "Which bird is the largest living species of bird in the world?",
        "a": "Emu",
        "b": "Ostrich",
        "c": "Albatross",
        "d": "Condor",
        "answer": "Ostrich"
    },
    {
        "text": "Who was the leader of the Soviet Union during the Cuban Missile Crisis in 1962?",
        "a": "Joseph Stalin",
        "b": "Leonid Brezhnev",
        "c": "Nikita Khrushchev",
        "d": "Mikhail Gorbachev",
        "answer": "Nikita Khrushchev"
    },
    {
        "text": "Who is the only tennis player to have won all four Grand Slam single titles in the same calendar year, known as the Calendar Slam, on multiple occasions?",
        "a": "Serena Williams",
        "b": "Rod Laver",
        "c": "Novak Djokovic",
        "d": "Björn Borg",
        "answer": "Rod Laver"
    },
    {
        "text": "In which city did the architectural style known as Brutalism first emerge in the 1950s?",
        "a": "Kiev",
        "b": "London",
        "c": "Moscow",
        "d": "Berlin",
        "answer": "London"
    },
    {
        "text": "What is the name of the process by which a caterpillar transforms into a butterfly?",
        "a": "Photosynthesis",
        "b": "Metamorphosis",
        "c": "Evolution",
        "d": "Migration",
        "answer": "Metamorphosis"
    },
    {
        "text": "Which ancient civilization developed one of the earliest known writing systems, called cuneiform?",
        "a": "The Babylonians",
        "b": "The Sumerians",
        "c": "The Phoenicians",
        "d": "The Persians",
        "answer": "The Sumerians"
    },
    {
        "text": "Who holds the record for the most race wins in Formula 1 history?",
        "a": "Michael Schumacher",
        "b": "Ayrton Senna",
        "c": "Lewis Hamilton",
        "d": "Sebastian Vettel",
        "answer": "Lewis Hamilton"
    },
    {
        "text": "The Nazca Lines are ancient geoglyphs carved into the desert floor, creating large drawings visible mainly from the air. In which country are they located?",
        "a": "Mexico",
        "b": "Bolivia",
        "c": "Chile",
        "d": "Peru",
        "answer": "Peru"
    },
    {
        "text": "Which famous battle in 1815 marked the final defeat of Napoleon?",
        "a": "Battle of Waterloo",
        "b": "Battle of Trafalgar",
        "c": "Battle of Austerlitz",
        "d": "Battle of Leipzig",
        "answer": "Battle of Waterloo"
    },
    {
        "text": "What were the first names of the famous Brothers Grimm, known for collecting fairy tales in the 19th century?",
        "a": "Johann and Friedrich",
        "b": "Wilhelm and Jacob",
        "c": "Karl and Heinrich",
        "d": "Ludwig and Otto",
        "answer": "Wilhelm and Jacob"
    },
    {
        "text": "How many days are there in a leap year?",
        "a": "364",
        "b": "365",
        "c": "366",
        "d": "367",
        "answer": "366"
    },
    {
        "text": "How many keys does a standard full-size piano generally have?",
        "a": "77",
        "b": "88",
        "c": "92",
        "d": "101",
        "answer": "88"
    },
    {
        "text": "What is the third decimal digit of the mathematical constant pi (π)?",
        "a": "1",
        "b": "4",
        "c": "5",
        "d": "9",
        "answer": "1"
    },
    {
        "text": "What is the most abundant gas in the Earth’s atmosphere?",
        "a": "Oxygen",
        "b": "Helium",
        "c": "Carbon dioxide",
        "d": "Nitrogen",
        "answer": "Nitrogen"
    },
    {
        "text": "Which football club has won the most UEFA Champions League titles in history?",
        "a": "AC Milan",
        "b": "Liverpool",
        "c": "Real Madrid",
        "d": "Bayern Munich",
        "answer": "Real Madrid"
    },
    {
        "text": "Which is the largest freshwater lake in the world by surface area?",
        "a": "Lake Superior",
        "b": "Lake Victoria",
        "c": "Lake Baikal",
        "d": "Lake Michigan",
        "answer": "Lake Superior"
    },
    {
        "text": "Which colour is the most common on the national flags of the world, featuring on 75% of national flags?",
        "a": "Red",
        "b": "Blue",
        "c": "Green",
        "d": "White",
        "answer": "Red"
    },
    {
        "text": "What is the stage name of Stefani Joanne Angelina Germanotta?",
        "a": "Madonna",
        "b": "Ariana Grande",
        "c": "Lady Gaga",
        "d": "Katy Perry",
        "answer": "Lady Gaga"
    },
    {
        "text": "What is the racing number of Lightning McQueen in the movie 'Cars'?",
        "a": "7",
        "b": "24",
        "c": "95",
        "d": "43",
        "answer": "95"
    },
    {
        "text": "In which country would you find Mecca?",
        "a": "Egypt",
        "b": "Syria",
        "c": "Iran",
        "d": "Saudi Arabia",
        "answer": "Saudi Arabia"
    },
    {
        "text": "What type of animal is Timon, in “The Lion King”?",
        "a": "Meerkat",
        "b": "Warthog",
        "c": "Mongoose",
        "d": "Lemur",
        "answer": "Meerkat"
    },
    {
        "text": "What was the original name that Mickey Mouse was supposed to have before it was changed and finalized?",
        "a": "Mortimer Mouse",
        "b": "Oswald Mouse",
        "c": "Charlie Mouse",
        "d": "George Mouse",
        "answer": "Mortimer Mouse"
    },
    {
        "text": "Where is the scaphoid bone located in the human body?",
        "a": "In the ankle",
        "b": "In the shoulder",
        "c": "In the knee",
        "d": "In the wrist",
        "answer": "In the wrist"
    },
    {
        "text": "Ted Kaczynski was better known as what?",
        "a": "The Zodiac Killer",
        "b": "The Unabomber",
        "c": "Jack the Ripper",
        "d": "The Big Lebowski",
        "answer": "The Unabomber"
    },
    {
        "text": "What is the most common religion traditionally practiced in China?",
        "a": "Buddhism",
        "b": "Christianity",
        "c": "Islam",
        "d": "Hinduism",
        "answer": "Buddhism"
    },
    {
        "text": "Which creature does Odysseus encounter that is described as a one-eyed giant in 'The Odyssey'?",
        "a": "Minotaur",
        "b": "Cyclops",
        "c": "Siren",
        "d": "Hydra",
        "answer": "Cyclops"
    },
    {
        "text": "On which date is St. Patrick's Day celebrated each year?",
        "a": "March 17",
        "b": "February 18",
        "c": "April 1",
        "d": "September 19",
        "answer": "March 17"
    },
    {
        "text": "What is the chemical symbol for the element antimony?",
        "a": "An",
        "b": "Tm",
        "c": "Sb",
        "d": "Am",
        "answer": "Sb"
    },
    {
        "text": "How many planets are there in the Solar System?",
        "a": "7",
        "b": "8",
        "c": "9",
        "d": "10",
        "answer": "8"
    },
    {
        "text": "What are human fingernails mainly made of?",
        "a": "Calcium",
        "b": "Keratin",
        "c": "Collagen",
        "d": "Enamel",
        "answer": "Keratin"
    },
    {
        "text": "What does the word 'karaoke' mean in Japanese?",
        "a": "Empty orchestra",
        "b": "Sing alone",
        "c": "Happy music",
        "d": "Live performance",
        "answer": "Empty orchestra"
    },
    {
        "text": "Bram Stoker, the author of 'Dracula', was from which nationality?",
        "a": "Irish",
        "b": "English",
        "c": "Scottish",
        "d": "Welsh",
        "answer": "Irish"
    },
    {
        "text": "What is the name of the famous clown in the novel and film 'It' by Stephen King?",
        "a": "Jack",
        "b": "Coulro",
        "c": "Bozo",
        "d": "Pennywise",
        "answer": "Pennywise"
    },
    {
        "text": "'Extra virgin' is considered the highest quality of which food product?",
        "a": "Wine",
        "b": "Olive oil",
        "c": "Cheese",
        "d": "Honey",
        "answer": "Olive oil"
    },
    {
        "text": "What is the name of the fictional African country where the movie 'Black Panther' is set?",
        "a": "Zambia",
        "b": "Wakanda",
        "c": "Nigeria",
        "d": "Ethiopia",
        "answer": "Wakanda"
    },
    {
        "text": "Which of these is used as a symbol of surrender or truce?",
        "a": "White flag",
        "b": "Blue banner",
        "c": "Green emblem",
        "d": "Yellow cross",
        "answer": "White flag"
    },
    {
        "text": "The golfer born Eldrick Tont Woods is best known by what first name?",
        "a": "Snake",
        "b": "Tiger",
        "c": "Vulture",
        "d": "Shark",
        "answer": "Tiger"
    },
    {
        "text": "The popular name of the ceremony seen outside Buckingham Palace is 'The changing of the ...' what?",
        "a": "Regiment",
        "b": "Soldiers",
        "c": "Troops",
        "d": "Guard",
        "answer": "Guard"
    },
    {
        "text": "When referring to a toilet, what do the letters WC stand for?",
        "a": "Wash Cupboard",
        "b": "Waste Chamber",
        "c": "Walled Cabinet",
        "d": "Water Closet",
        "answer": "Water Closet"
    },
    {
        "text": "What is the name of the main award given at the Cannes Film Festival?",
        "a": "Golden Lion",
        "b": "Palme d'Or",
        "c": "Golden Bear",
        "d": "Silver Screen",
        "answer": "Palme d'Or"
    },
    {
        "text": "What is the term for a young horse?",
        "a": "Foal",
        "b": "Stallion",
        "c": "Mare",
        "d": "Pony",
        "answer": "Foal"
    },
    {
        "text": "What is the longest railway tunnel in the world?",
        "a": "Channel Tunnel",
        "b": "Seikan Tunnel",
        "c": "Gotthard Base Tunnel",
        "d": "Lærdal Tunnel",
        "answer": "Gotthard Base Tunnel"
    },
    {
        "text": "Who was born first among these four people?",
        "a": "Sigmund Freud",
        "b": "Albert Einstein",
        "c": "Vincent van Gogh",
        "d": "Fyodor Dostoevsky",
        "answer": "Fyodor Dostoevsky"
    },
    {
        "text": "Which hat with a soft brim and indented crown became popular in the early 20th century and is often associated with detectives and gangsters?",
        "a": "Fedora",
        "b": "Boater",
        "c": "Bowler hat",
        "d": "Deerstalker",
        "answer": "Fedora"
    },
    {
        "text": "What is the name of Thor’s powerful hammer in Norse mythology?",
        "a": "Gungnir",
        "b": "Mjölnir",
        "c": "Gram",
        "d": "Skofnung",
        "answer": "Mjölnir"
    },
    {
        "text": "What is the name of the main character in the video game series 'Tomb Raider'?",
        "a": "Samus Aran",
        "b": "Jill Valentine",
        "c": "Lara Croft",
        "d": "Aloy",
        "answer": "Lara Croft"
    },
    {
        "text": "Which letter was the last one added to the English alphabet?",
        "a": "J",
        "b": "W",
        "c": "Y",
        "d": "Z",
        "answer": "J"
    },
    {
        "text": "In which city is Nicolas Flamel traditionally said to have lived and worked as a scribe and alchemist?",
        "a": "Paris",
        "b": "London",
        "c": "Turin",
        "d": "Prague",
        "answer": "Paris"
    },
    {
        "text": "What is the second closest star to Earth after the Sun?",
        "a": "Proxima Centauri",
        "b": "Sirius",
        "c": "Alpha Centauri A",
        "d": "Betelgeuse",
        "answer": "Proxima Centauri"
    },
    {
        "text": "Which famous castle in Romania was associated with the legend of Dracula by the communist regime during the late 20th century?",
        "a": "Hohenzollern Castle",
        "b": "Neuschwanstein Castle",
        "c": "Edinburgh Castle",
        "d": "Bran Castle",
        "answer": "Bran Castle"
    },
    {
        "text": "Who was Anastasia Romanov?",
        "a": "The daughter of the last Emperor of Russia",
        "b": "A Russian Tsarina",
        "c": "A famous ballet dancer",
        "d": "A novelist from Ukraine",
        "answer": "The daughter of the last Emperor of Russia"
    },
    {
        "text": "Which breed of dog is renowned for its spotted coat?",
        "a": "Dalmatian",
        "b": "Boxer",
        "c": "Rottweiler",
        "d": "Golden Retriever",
        "answer": "Dalmatian"
    },
    {
        "text": "Which topping famously features on a Hawaiian pizza?",
        "a": "Egg",
        "b": "Chicken",
        "c": "Pineapple",
        "d": "Anchovies",
        "answer": "Pineapple"
    },
    {
        "text": "Known by the abbreviation TNT, trinitrotoluene is commonly used as a type of what?",
        "a": "Adhesive",
        "b": "Explosive",
        "c": "Insulator",
        "d": "Lubricant",
        "answer": "Explosive"
    },
    {
        "text": "What is the nickname of the US city of Las Vegas",
        "a": "The Big Easy",
        "b": "The Wind City",
        "c": "The Big Apple",
        "d": "Sin City",
        "answer": "Sin City"
    },
    {
        "text": "Where did Steve Jobs work before co-founding Apple?",
        "a": "Microsoft",
        "b": "IBM",
        "c": "Google",
        "d": "Atari",
        "answer": "Atari"
    },
    {
        "text": "What was the name of the animated film studio founded by Steve Jobs in 1986?",
        "a": "Pixar",
        "b": "DreamWorks",
        "c": "Illumination",
        "d": "Blue Sky Studios",
        "answer": "Pixar"
    },
    {
        "text": "Which 2020s TV series is set in a post-apocalyptic world and is based on a popular video game?",
        "a": "The Crown",
        "b": "Stranger Things",
        "c": "The Last of Us",
        "d": "Wednesday",
        "answer": "The Last of Us"
    },
    {
        "text": "Who was the main actor who played Will in the TV series 'The Fresh Prince of Bel-Air'?",
        "a": "Will Smith",
        "b": "Martin Lawrence",
        "c": "Eddie Murphy",
        "d": "Jamie Foxx",
        "answer": "Will Smith"
    },
    {
        "text": "In 2019, MacKenzie Scott divorced which US businessman with a reported settlement of $38 billion?",
        "a": "Jeff Bezos",
        "b": "Larry Page",
        "c": "Elon Musk",
        "d": "Mark Zuckerberg",
        "answer": "Jeff Bezos"
    },
    {
        "text": "Which country is traditionally associated with the beer style called Pilsner?",
        "a": "Germany",
        "b": "Belgium",
        "c": "Czech Republic",
        "d": "Ireland",
        "answer": "Czech Republic"
    },
    {
        "text": "What does the French word 'calembour' mean in English?",
        "a": "A serious speech",
        "b": "A type of dance",
        "c": "A play on words",
        "d": "A poem",
        "answer": "A play on words"
    },
    {
        "text": "In which country was absinthe first created?",
        "a": "France",
        "b": "Italy",
        "c": "Switzerland",
        "d": "Spain",
        "answer": "Switzerland"
    },
    {
        "text": "Who painted 'Impression, Sunrise' (Impression, soleil levant), the painting that gave name to Impressionism?",
        "a": "Claude Monet",
        "b": "Édouard Manet",
        "c": "Pierre-Auguste Renoir",
        "d": "Paul Cézanne",
        "answer": "Claude Monet"
    },
    {
        "text": "Which novel has a first chapter titled 'It was a pleasure to burn'?",
        "a": "1984",
        "b": "Brave New World",
        "c": "Fahrenheit 451",
        "d": "The Great Gatsby",
        "answer": "Fahrenheit 451"
    },
    {
        "text": "The Sahara desert covers a large section of which of these countries?",
        "a": "Cameroon",
        "b": "Mali",
        "c": "Ghana",
        "d": "Somalia",
        "answer": "Mali"
    },
    {
        "text": "On which day is the Mexican Independence Day (Grito de Dolores) celebrated?",
        "a": "February 24th",
        "b": "May 5th",
        "c": "September 16th",
        "d": "November 2nd",
        "answer": "September 16th"
    },
    {
        "text": "In a popular Spanish New Year's Eve tradition, revelers attempt to eat and swallow 12 of which food before the midnight bell tolls 12 times?",
        "a": "Pimientos",
        "b": "Grapes",
        "c": "Anchovies",
        "d": "Hazelnuts",
        "answer": "Grapes"
    },
    {
        "text": "Believed to help them conserve energy, \"vertical sleeping\" is a unique behavior exhibited by which of these animals?",
        "a": "Sperm whale",
        "b": "Bactrian camel",
        "c": "Canada goose",
        "d": "Ring-tailed lemur",
        "answer": "Sperm whale"
    },
    {
        "text": "Which of the following cities is considered the oldest, now inhabited, city in the world?",
        "a": "Athens (Greece)",
        "b": "Diyarbakir (Turkey)",
        "c": "Jericho (Palestine)",
        "d": "Byblos (Lebanon)",
        "answer": "Jericho (Palestine)"
    },
    {
        "text": "John Spilsbury invented the first jigsaw puzzles in London around 1760. What was his profession?",
        "a": "Carpenter",
        "b": "Cartographer",
        "c": "Teacher",
        "d": "Printer",
        "answer": "Cartographer"
    },
    {
        "text": "Niccolò Paganini was famous for playing the violin and the viola. Which other instrument did he also play?",
        "a": "Cello",
        "b": "Flute",
        "c": "Piano",
        "d": "Guitar",
        "answer": "Guitar"
    },
    {
        "text": "Who is generally considered the first superhero in newspaper comic strip?",
        "a": "Hugo Hercules",
        "b": "Superman",
        "c": "Fantomah",
        "d": "Captain America",
        "answer": "Hugo Hercules"
    },
    {
        "text": "Which figure from Greek mythology is often depicted carrying the globe on his shoulders?",
        "a": "Atlas",
        "b": "Hermes",
        "c": "Hyperion",
        "d": "Zeus",
        "answer": "Atlas"
    },
    {
        "text": "The Large Hadron Collider (LHC) at CERN in Geneva accelerates subatomic particles to close to the speed of light using what?",
        "a": "Superconductive magnets",
        "b": "Nuclear propulsion",
        "c": "Rocket turbine engines",
        "d": "Compressed liquids",
        "answer": "Superconductive magnets"
    },
    {
        "text": "The first artificial nuclear reactor, led by Enrico Fermi, achieved the first self-sustaining nuclear chain reaction on December 2, 1942. At the university of which city did this take place?",
        "a": "Chicago",
        "b": "New York",
        "c": "Paris",
        "d": "Berlin",
        "answer": "Chicago"
    },
    {
        "text": "Which of these gemstones is NOT typically red?",
        "a": "Ruby",
        "b": "Spinel",
        "c": "Tanzanite",
        "d": "Garnet",
        "answer": "Tanzanite"
    },
    {
        "text": "What was traditionally hidden inside many Fabergé eggs created for the Russian imperial family?",
        "a": "A small mechanical surprise",
        "b": "A gold coin",
        "c": "A secret letter",
        "d": "A perfume bottle",
        "answer": "A small mechanical surprise"
    },
    {
        "text": "In Hokusai's print 'The Great Wave off Kanagawa', which mountain is depicted in the background?",
        "a": "Mount Emei",
        "b": "Mount Kailash",
        "c": "Mount Fuji",
        "d": "Mount Sinai",
        "answer": "Mount Fuji"
    },
    {
        "text": "What is the best possible score in Blackjack?",
        "a": "10",
        "b": "21",
        "c": "42",
        "d": "77",
        "answer": "21"
    },
    {
        "text": "Which of the following is NOT botanically considered a berry?",
        "a": "Tomato",
        "b": "Lemon",
        "c": "Strawberry",
        "d": "Banana",
        "answer": "Strawberry"
    },
    {
        "text": "Which country is the Chihuahua dog breed originally from?",
        "a": "Spain",
        "b": "Argentina",
        "c": "Brazil",
        "d": "Mexico",
        "answer": "Mexico"
    },
    {
        "text": "What was the first Disney film to feature live actors?",
        "a": "Snow White and the Seven Dwarfs",
        "b": "Song of the South",
        "c": "Cinderella",
        "d": "Fantasia",
        "answer": "Song of the South"
    },
    {
        "text": "On which mountain did Moses receive the Ten Commandments?",
        "a": "Mount Sinai",
        "b": "Mount Olympus",
        "c": "Mount Golgotha",
        "d": "Mount Ararat",
        "answer": "Mount Sinai"
    },
    {
        "text": "Which of the following explorers was NOT Italian?",
        "a": "Cristoforo Colombo",
        "b": "Ferdinando Magellano",
        "c": "Amerigo Vespucci",
        "d": "Marco Polo",
        "answer": "Ferdinando Magellano"
    },
    {
        "text": "What happened in October 1852 in some countries to switch from the Julian calendar to the Gregorian calendar?",
        "a": "An extra month was added",
        "b": "The week was shortened to 5 days",
        "c": "The year changed to 1853",
        "d": "Ten days were skipped",
        "answer": "Ten days were skipped"
    },
    {
        "text": "During the 1932 conflict against whom did the Australian army have to withdraw?",
        "a": "A group of local rebels",
        "b": "The lunar eclipse",
        "c": "A white shark",
        "d": "A mob of Emus",
        "answer": "A mob of Emus"
    },
    {
        "text": "Which of the following sports has never been part of the Olympic Games?",
        "a": "Badminton",
        "b": "Breakdance",
        "c": "Chess",
        "d": "Golf",
        "answer": "Chess"
    },
    {
        "text": "Which of the following numbers is the largest?",
        "a": "3",
        "b": "π (pi)",
        "c": "√10",
        "d": "e",
        "answer": "√10"
    },
    {
        "text": "Which of the following numbers is the smallest?",
        "a": "3",
        "b": "π (pi)",
        "c": "√10",
        "d": "e",
        "answer": "e"
    },
    {
        "text": "In the Italian 'brainrot' meme, what kind of animal is Trallalero Trallalà?",
        "a": "Monkey",
        "b": "Crocodile",
        "c": "Shark",
        "d": "Bear",
        "answer": "Shark"
    },
    {
        "text": "What is the name of the protagonist of 'The Count of Monte Cristo'?",
        "a": "Edmond Dantès",
        "b": "Jean Valjean",
        "c": "D'Artagnan",
        "d": "Phileas Fogg",
        "answer": "Edmond Dantès"
    },
    {
        "text": "Who wrote the play 'Death of a Salesman'?",
        "a": "Tennessee Williams",
        "b": "Arthur Miller",
        "c": "Eugene O'Neill",
        "d": "Samuel Beckett",
        "answer": "Arthur Miller"
    },
    {
        "text": "Which American author and physician wrote the techno-thriller 'Jurassic Park' and created the hit TV series 'ER'?",
        "a": "Stephen King",
        "b": "James Patterson",
        "c": "Dan Brown",
        "d": "Michael Crichton",
        "answer": "Michael Crichton"
    },
    {
        "text": "Who composed the opera 'The Magic Flute' (Die Zauberflöte)?",
        "a": "Ludwig van Beethoven",
        "b": "Giuseppe Verdi",
        "c": "Wolfgang Amadeus Mozart",
        "d": "Richard Wagner",
        "answer": "Wolfgang Amadeus Mozart"
    },
    {
        "text": "Who was the first emperor of Rome?",
        "a": "Julius Caesar",
        "b": "Augustus",
        "c": "Nero",
        "d": "Trajan",
        "answer": "Augustus"
    },
    {
        "text": "Which ancient leader is known for crossing the Alps with elephants?",
        "a": "Julius Caesar",
        "b": "Alexander the Great",
        "c": "Hannibal",
        "d": "Cleopatra",
        "answer": "Hannibal"
    },
    {
        "text": "Which treaty, signed in 1494, divided newly discovered lands outside Europe between the Portuguese Empire and the Crown of Castile?",
        "a": "Treaty of Versailles",
        "b": "Treaty of Tordesillas",
        "c": "Treaty of Utrecht",
        "d": "Treaty of Westphalia",
        "answer": "Treaty of Tordesillas"
    },
    {
        "text": "Which event marks the beginning of the French Revolution?",
        "a": "The execution of King Louis XVI",
        "b": "The Storming of the Bastille",
        "c": "The death of the Sun King (Louis XIV)",
        "d": "The crowning of Napoleon Bonaparte",
        "answer": "The Storming of the Bastille"
    },
    {
        "text": "In which modern-day country are the ruins of the ancient city of Carthage located?",
        "a": "Egypt",
        "b": "Libya",
        "c": "Tunisia",
        "d": "Greece",
        "answer": "Tunisia"
    },
    {
        "text": "What was the name of the large-scale religious military campaigns launched during the Middle Ages to the Holy Land?",
        "a": "The Renaissance",
        "b": "The Inquisitions",
        "c": "The Crusades",
        "d": "The Reformation",
        "answer": "The Crusades"
    },
    {
        "text": "Which pandemic spread across Europe in the 14th century during the Middle Ages?",
        "a": "Spanish Flu",
        "b": "Smallpox",
        "c": "The Black Death",
        "d": "Cholera",
        "answer": "The Black Death"
    },
    {
        "text": "Quali patatine mi piacciono tra patatine alla paprika e patatine al pepe rosa?",
        "a": "Patatine alla paprika",
        "b": "Patatine al pepe rosa",
        "c": "Patatine al Anna Pepe",
        "d": "Giusto",
        "answer": "Giusto"
    },
    {
        "text": "What does the Sanskrit word 'Mahātmā', the famous nickname given to Gāndhī, literally mean?",
        "a": "Great soul",
        "b": "Awakened one",
        "c": "Wise teacher",
        "d": "Holy king",
        "answer": "Great soul"
    },
    {
        "text": "What did Mohāndās Karamchand Gāndhī study in London?",
        "a": "Medicine",
        "b": "Philosophy",
        "c": "Engineering",
        "d": "Law",
        "answer": "Law"
    },
    {
        "text": "In which city was Bruce Lee born?",
        "a": "Hong Kong",
        "b": "San Francisco",
        "c": "Beijing",
        "d": "Los Angeles",
        "answer": "San Francisco"
    },
    {
        "text": "In 'Chungking Express', which food item does Cop 223 obsessively buy because of its expiration date?",
        "a": "Instant noodles",
        "b": "Pineapple cans",
        "c": "Chocolate bars",
        "d": "Milk cartons",
        "answer": "Pineapple cans"
    },
    {
        "text": "What was the name of the filmmaking manifesto launched in 1995 by Danish directors Lars von Trier and Thomas Vinterberg?",
        "a": "New Wave",
        "b": "Dogme 95",
        "c": "Cinema Verité",
        "d": "Nordic Realism",
        "answer": "Dogme 95"
    },
    {
        "text": "Produced in the pineal gland, which hormone plays an important role in the regulation of the human sleep cycle?",
        "a": "Melatonin",
        "b": "Oxytocin",
        "c": "Thyroxine",
        "d": "Insulin",
        "answer": "Melatonin"
    },
    {
        "text": "The Mekong River delta is located in which Asian country?",
        "a": "Vietnam",
        "b": "Myanmar",
        "c": "Thailand",
        "d": "Laos",
        "answer": "Vietnam"
    },
    {
        "text": "Which island is among the first places in the world to celebrate the New Year because of its time zone?",
        "a": "Hawaii",
        "b": "Kiribati",
        "c": "New Zealand",
        "d": "Greenland",
        "answer": "Kiribati"
    },
    {
        "text": "Approximately how many teeth can a shark lose and replace in its lifetime?",
        "a": "About 100",
        "b": "About 500",
        "c": "About 5,000",
        "d": "More than 20,000",
        "answer": "More than 20,000"
    },
    {
        "text": "Wich enzyme is important in the process of blood clotting?",
        "a": "Thrombin",
        "b": "Amylase",
        "c": "Trypsin",
        "d": "Pepsin",
        "answer": "Thrombin"
    },
    {
        "text": "Which of these cities is closest to the Equator?",
        "a": "New Delhi",
        "b": "Il Cairo",
        "c": "Sydney",
        "d": "Buenos Aires",
        "answer": "New Delhi"
    },
    {
        "text": "Which of these cities is located the farthest south?",
        "a": "New York",
        "b": "Naples",
        "c": "Sofia",
        "d": "Barcelona",
        "answer": "New York"
    },
    {
        "text": "Who is the author of the quote 'In the future, everyone will be world-famous for 15 minutes'?",
        "a": "Pablo Picasso",
        "b": "Andy Warhol",
        "c": "Salvador Dalí",
        "d": "Marcel Duchamp",
        "answer": "Andy Warhol"
    },
    {
        "text": "Which film ranked first in the 2008 Empire magazine list of the 500 greatest films of all time?",
        "a": "The Godfather",
        "b": "2001: A Space Odyssey",
        "c": "Casablanca",
        "d": "Citizen Kane",
        "answer": "The Godfather"
    },
    {
        "text": "Who is widely considered the richest person in history when adjusting for inflation?",
        "a": "Augustus Caesar",
        "b": "Elon Musk",
        "c": "Mansa Musa",
        "d": "John D. Rockefeller",
        "answer": "Mansa Musa"
    },
    {
        "text": "What indication from the Vatican officially signals that a new Pope has been elected?",
        "a": "The ringing of the bells of St. Peter's Basilica",
        "b": "The release of white smoke from the Sistine Chapel chimney",
        "c": "A white flag raised over the Apostolic Palace",
        "d": "A cannon blast from Castel Sant'Angelo",
        "answer": "The release of white smoke from the Sistine Chapel chimney"
    },
    {
        "text": "'This Is Me' was a 2018 hit single from which musical film?",
        "a": "A Star Is Born",
        "b": "The Greatest Showman",
        "c": "La La Land",
        "d": "Mamma Mia! Here We Go Again",
        "answer": "The Greatest Showman"
    },
    {
        "text": "Which of these slogans is used by a fast food company that specializes in chicken?",
        "a": "I'm lovin' it",
        "b": "Just eat it",
        "c": "Eat fresh",
        "d": "Finger lickin' good",
        "answer": "Finger lickin' good"
    },
    {
        "text": "Who has won the most Olympic gold medals in history?",
        "a": "Michael Phelps",
        "b": "Usain Bolt",
        "c": "Johannes Høsflot Klæbo",
        "d": "Larissa Latynina",
        "answer": "Michael Phelps"
    },
    {
        "text": "According to 2025 United Nations estimates, which city has the largest population in the world (nearly 42 million inhabitants)?",
        "a": "Tokyo (Japan)",
        "b": "Delhi (India)",
        "c": "Jakarta (Indonesia)",
        "d": "Dhaka (Bangladesh)",
        "answer": "Jakarta (Indonesia)"
    },
    {
        "text": "Who was the first bodybuilder to win 8 Mr. Olympia titles?",
        "a": "Arnold Schwarzenegger",
        "b": "Ronnie Coleman",
        "c": "Lee Haney",
        "d": "Phil Heath",
        "answer": "Lee Haney"
    }, 
    {
        "text": "Which of the following is NOT an acronym?",
        "a": "NASA",
        "b": "SOS",
        "c": "Laser",
        "d": "CAPTCHA",
        "answer": "SOS"
    },
    {
        "text": "To whom is the quote 'If they have no bread, let them eat cake' falsely attributed?",
        "a": "Marie Antoinette",
        "b": "Catherine the Great",
        "c": "Mary Stuart",
        "d": "Louis XVI",
        "answer": "Marie Antoinette"
    },
    {
        "text": "Which rival company was founded by Rudolf Dassler, the brother of the founder of Adidas?",
        "a": "Puma",
        "b": "Nike",
        "c": "Reebok",
        "d": "Hugo Boss",
        "answer": "Puma"
    },
    {
  "text": "Which is considered the oldest car manufacturer still in operation in the world?",
        "a": "Mercedes-Benz",
        "b": "Peugeot",
        "c": "Ford",
        "d": "Opel",
        "answer": "Peugeot"
    },
    {
        "text": "Which of these abbreviations represents a qualification achived in higher education?",
        "a": "AKA",
        "b": "TBC",
        "c": "PhD",
        "d": "GBH",
        "answer": "PhD"
    },
    {
        "text": "Which novel starts with the line: \"When he was nearly thirteen my brother Jem got his arm badly broken at the elbow\"?",
        "a": "The Great Gatsby",
        "b": "To Kill a Mockingbird",
        "c": "The Catcher in the Rye",
        "d": "On the Road",
        "answer": "To Kill a Mockingbird"
    },
    {
        "text": "In the 1838 photograph 'Boulevard du Temple', widely considered the first photograph to include a human, who was the person captured in the image?",
        "a": "Louis Daguerre",
        "b": "A man having his shoes shined",
        "c": "Napoleon III",
        "d": "A police officer",
        "answer": "A man having his shoes shined"
    },
    {
        "text": "Who is widely reported to have one of the highest officially recorded IQ scores in history, estimated between 180 and 230?",
        "a": "Albert Einstein",
        "b": "Terence Táo",
        "c": "Stephen Hawking",
        "d": "Isaac Newton",
        "answer": "Terence Táo"
    },
    {
        "text": "The famous 1950 Paris photograph 'Le Baiser de l'Hôtel de Ville' by Robert Doisneau represented:",
        "a": "A candid shot of a man falling on the street",
        "b": "A staged photo of two aspiring actors kissing",
        "c": "A secret snapshot of a famous French politician",
        "d": "A frame taken from a French New Wave movie",
        "answer": "A staged photo of two aspiring actors kissing"
    },
    {
        "text": "Which painting is considered the most expensive artwork ever sold at auction up to 2026, selling for about $450 million in 2017?",
        "a": "The Starry Night by Vincent van Gogh",
        "b": "Salvator Mundi by Leonardo da Vinci",
        "c": "The Card Players by Paul Cézanne",
        "d": "Interchange by Willem de Kooning",
        "answer": "Salvator Mundi by Leonardo da Vinci"
    },
    {
        "text": "The famous Berlin Wall mural depicting a 'fraternal kiss' features Leonid Brezhnev and which other political leader?",
        "a": "Erich Honecker",
        "b": "Mikhail Gorbachev",
        "c": "Nikita Khrushchev",
        "d": "John F. Kennedy",
        "answer": "Erich Honecker"
    },
    {
        "text": "The famous 1932 photograph 'Lunch atop a Skyscraper' showing 11 construction workers eating their meal while sitting on a steel beam 800 feet above the ground was taken during the construction of which building?",
        "a": "Empire State Building",
        "b": "Chrysler Building",
        "c": "Rockefeller Center",
        "d": "Space Needle",
        "answer": "Rockefeller Center"
    },
    {
        "text": "What was Che Guevara's profession before becoming a revolutionary?",
        "a": "Lawyer",
        "b": "Doctor",
        "c": "Teacher",
        "d": "Engineer",
        "answer": "Doctor"
    },
    {
        "text": "What nickname is the famous skateboarder Tony Hawk commonly known by?",
        "a": "The Falcon",
        "b": "The Birdman",
        "c": "The Skater King",
        "d": "The Hawk Eye",
        "answer": "The Birdman"
    },
    {
        "text": "Jolifanto bambla ô falli bambla",
        "a": "Hugo Ball",
        "b": "Tristan Tzara",
        "c": "Marcel Duchamp",
        "d": "Man Ray",
        "answer": "Hugo Ball"
    },
    {
        "text": "In which country is Waterloo, the site of Napoleon's final defeat in 1815, located?",
        "a": "France",
        "b": "Belgium",
        "c": "Netherlands",
        "d": "Germany",
        "answer": "Belgium"
    },
    {
        "text": "Which of these writers did NOT use a pen name?",
        "a": "Mark Twain",
        "b": "Italo Svevo",
        "c": "J.K. Rowling",
        "d": "Ernest Hemingway",
        "answer": "Ernest Hemingway"
    },
    {
        "text": "The Watergate scandal, which involved Richard Nixon in the 1970s, takes its name from what?",
        "a": "A secret government project",
        "b": "A hotel and office complex in Washington, D.C.",
        "c": "A newspaper investigation journal",
        "d": "A water purification system",
        "answer": "A hotel and office complex in Washington, D.C."
    },
    {
        "text": "What is a velocipede?",
        "a": "An early type of bicycle",
        "b": "A type of train",
        "c": "A sailing ship",
        "d": "A horse-drawn carriage",
        "answer": "An early type of bicycle"
    },
    {
        "text": "Which language is widely considered to have the largest vocabulary (number of unique words) in the world?",
        "a": "Mandarin Chinese",
        "b": "English",
        "c": "Italian",
        "d": "Portuguese",
        "answer": "English"
    },
    {
        "text": "Which of the following events occurred first in history?",
        "a": "The completion of the Great Pyramid of Giza",
        "b": "The total extinction of the last Woolly Mammoths",
        "c": "The rise of the Babylonian Empire under Hammurabi",
        "d": "The invention of the first alphabet by the Phoenicians",
        "answer": "The completion of the Great Pyramid of Giza"
    },
    {
        "text": "In the world of 'The Simpsons', what is unique about the appearance of God?",
        "a": "He is the only character with purple eyes",
        "b": "He is the only character with five fingers on each hand",
        "c": "He is the only character who is drawn in 3D",
        "d": "He is the only character who doesn't wear shoes",
        "answer": "He is the only character with five fingers on each hand"
    },
    {
        "text": "The letter 'E' is the international veichle registration code for which country?",
        "a": "Spain",
        "b": "Luxembourg",
        "c": "Norway",
        "d": "Lithuania",
        "answer": "Spain"
    },
    {
        "text": "What was the birth name of the boxer known as Muhammad Ali?",
        "a": "Cassius Clay",
        "b": "Malcolm X",
        "c": "Joe Frazier",
        "d": "George Foreman",
        "answer": "Cassius Clay"
    },
    {
        "text": "In which year did the Concorde make its last commercial flight?",
        "a": "1998",
        "b": "2003",
        "c": "2008",
        "d": "2013",
        "answer": "2003"
    },
    {
        "text": "The Mall of the Emirates in Dubai is world-famous for housing which permanent attraction inside its complex?",
        "a": "The world's largest indoor coral reef",
        "b": "A full-sized indoor ski resort with real snow",
        "c": "A working replica of the Venice canals",
        "d": "A giant indoor rainforest with free-flying tropical birds",
        "answer": "A full-sized indoor ski resort with real snow"
    },
    {
        "text": "Which of the following locations does NOT feature a famous man-made ship canal cutting through its land?",
        "a": "Panama",
        "b": "Suez",
        "c": "Ural",
        "d": "Corinth",
        "answer": "Ural"
    },
    {
        "text": "Which of the following pairs of national capitals are the closest to each other?",
        "a": "Pyongyang and Seoul",
        "b": "Buenos Aires and Montevideo",
        "c": "Brussels and Amsterdam",
        "d": "Vienna and Bratislava",
        "answer": "Vienna and Bratislava"
    },
    {
        "text": "Which of the following is considered the most remote inhabited island on Earth, located over 2,400 km from the nearest mainland?",
        "a": "Easter Island",
        "b": "Point Nemo",
        "c": "Saint Helena",
        "d": "Tristan da Cunha",
        "answer": "Tristan da Cunha"
    },
    {
        "text": "What is an ambigram in typography and graphic design?",
        "a": "A word that reads the same forward and backward",
        "b": "A font designed to be readable by people with dyslexia",
        "c": "A word formed by rearranging the letters of another",
        "d": "A calligraphic design that can be read from different orientations",
        "answer": "A calligraphic design that can be read from different orientations"
    },
    {
        "text": "In 1893, which was the first self-governing nation in the world to grant all women the right to vote?",
        "a": "Switzerland",
        "b": "Finland",
        "c": "United States",
        "d": "New Zealand",
        "answer": "New Zealand"
    },
    {
        "text": "What is the final word of the Bible in most standard English translations?",
        "a": "Lord",
        "b": "Forever",
        "c": "Hell",
        "d": "Amen",
        "answer": "Amen"
    },
    {
        "text": "Which of the following is NOT a traditional Japanese dish?",
        "a": "Sashimi",
        "b": "Unagi",
        "c": "Ramen",
        "d": "Tatami",
        "answer": "Tatami"
    },
    {
        "text": "One of the best-selling children's book of all time, 'The Little Prince' was written and illustrated by which French author?",
        "a": "René Goscinny",
        "b": "Jean de Brunhoff",
        "c": "Charles Perrault",
        "d": "Antoine de Saint-Exupéry",
        "answer": "Antoine de Saint-Exupéry"
    },
    {
        "text": "In L. Frank Baum's original 1900 novel, 'The Wonderful Wizard of Oz', what color are Dorothy's magical shoes?",
        "a": "Ruby",
        "b": "Emerald green",
        "c": "Gold",
        "d": "Silver",
        "answer": "Silver"
    },
    {
        "text": "Which of the following terms does NOT refer to a legal move or a specific term in the game of chess?",
        "a": "En passant",
        "b": "Castling",
        "c": "Gambit",
        "d": "Foul",
        "answer": "Foul"
    },
    {
        "text": "The wireless technology 'Bluetooth' was named after what?",
        "a": "A 10th-century Viking King",
        "b": "The tinted lens used in the first signal receiver",
        "c": "A rare species of bird",
        "d": "An acronym",
        "answer": "A 10th-century Viking King"
    },
    {
        "text": "Who is widely reported to have earned the highest salary for a single film production, totaling over $156 million?",
        "a": "Robert Downey Jr. (Avengers: Endgame)",
        "b": "Tom Cruise (Top Gun: Maverick)",
        "c": "Keanu Reeves (The Matrix Reloaded & Revolutions)",
        "d": "Bruce Willis (The Sixth Sense)",
        "answer": "Keanu Reeves (The Matrix Reloaded & Revolutions)"
    },
    {
        "text": "In Boris Pasternak's masterpiece 'Doctor Zhivago', which historical event serves as the primary backdrop that upends the lives of the characters?",
        "a": "The Napoleonic Wars",
        "b": "The Decembrist Revolt",
        "c": "The Russian Revolution and Civil War",
        "d": "The Crimean War",
        "answer": "The Russian Revolution and Civil War"
    },
    {
        "text": "Which famous artist designed the iconic daisy-shaped logo for the Chupa Chups lollipop brand in 1969?",
        "a": "Julio Iglesias",
        "b": "Andy Warhol",
        "c": "Ignacio Chups",
        "d": "Salvador Dalí",
        "answer": "Salvador Dalí"
    },
    {
        "text": "The Japanese word 'sushi' is derived from an archaic grammatical form that literally means what?",
        "a": "Raw fish",
        "b": "Hand pressed",
        "c": "Vinegar rice",
        "d": "Sour tasting",
        "answer": "Sour tasting"
    },
    {
        "text": "The word 'Golf' derives from what?",
        "a": "The acronym: Gentlemen Only, Ladies Forbidden",
        "b": "An arcaic synonym of: 'To hit or strike'",
        "c": "A word describing the green open land fields",
        "d": "Dutch and Scottish words meaning: Club or stick",
        "answer": "Dutch and Scottish words meaning: Club or stick"
    },
    {
        "text": "The famous 'Caesar Salad' dish was named after?",
        "a": "Cesare Cardini, an Italian-Mexican restaurateur",
        "b": "Caesar Ritz, the famous Swiss hotelier",
        "c": "the Roman Emperor Julius Caesar",
        "d": "Little Caesars, the American pizza chain",
        "answer": "Cesare Cardini, an Italian-Mexican restaurateur"
    },
    {
        "text": "Where were the 'fortune cookies', cookies with a small message inside, actually invented?",
        "a": "China",
        "b": "India",
        "c": "USA",
        "d": "Japan",
        "answer": "Japan"
    },
    {
        "text": "The word 'ketchup' is believed to have originated from a term referring to what?",
        "a": "A tomato-based sauce in Germany",
        "b": "A type of vinegar in France",
        "c": "The phrase 'catch up' in English",
        "d": "A fermented fish sauce in Chinese",
        "answer": "A fermented fish sauce in Chinese"
    },
    {
        "text": "The world's most expensive liqueur, valued at approximately 32 million euros, featuring four diamonds embedded in its bottle, is a version of which drink?",
        "a": "An Italian Limoncello",
        "b": "A French Vodka",
        "c": "A Mexican Tequila",
        "d": "A Scottish Whisky",
        "answer": "An Italian Limoncello"
    },
    {
        "text": "The 'high five' 🙌 is widely believed to have been invented by who?",
        "a": "The ancient Romans",
        "b": "A group of surfers in Hawaii",
        "c": "Napoleon Bonaparte after a battle victory",
        "d": "Two baseball players in 1977",
        "answer": "Two baseball players in 1977"
    },
    {
        "text": "In 1992, the government of Singapore made international headlines by officially banning what?",
        "a": "Chewing gum",
        "b": "Singing in public",
        "c": "Dogs as pets",
        "d": "Using plastic straws",
        "answer": "Chewing gum"
    },
    {
        "text": "John Cage's composition 4'33\" is famous because:",
        "a": "It contains exactly 433 notes",
        "b": "It uses only one instrument",
        "c": "It is played entirely underwater",
        "d": "It consists of 273 seconds of silence",
        "answer": "It consists of 273 seconds of silence"
    },
    {
        "text": "Who is widely credited with performing one of the first major stadium music concert?",
        "a": "The Beatles in New York City",
        "b": "The Who in London",
        "c": "The Rolling Stones in London",
        "d": "Elvis Presley in Washington",
        "answer": "Elvis Presley in Washington"
    },
    {
        "text": "In which city is William Shakespeare's famous tragedy 'Hamlet' set?",
        "a": "Heidelberg, Germany",
        "b": "Vienna, Austria",
        "c": "Verona, Italy",
        "d": "Elsinore, Denmark",
        "answer": "Elsinore, Denmark"
    },
    {
        "text": "William Shakespeare is credited with inventing or first recording thousands of words in the English language. Which of these common words was coined by him?",
        "a": "Eyeglasses",
        "b": "Infant",
        "c": "Telephone",
        "d": "Addiction",
        "answer": "Addiction"
    },
    {
        "text": "Halley's Comet has an orbital period between 20 and 200 years. When was it last observed from Earth at its perihelion?",
        "a": "1934",
        "b": "1986",
        "c": "2001",
        "d": "2010",
        "answer": "1986"
    },
    {
        "text": "Which animal is responsible for the most human deaths per year through direct causes, after mosquitoes?",
        "a": "Elephants",
        "b": "Dogs",
        "c": "Hippopotamus",
        "d": "Snakes",
        "answer": "Snakes"
    },
    {
        "text": "Which Roman Emperor officially declared Christianity the state religion of the Roman Empire with the Edict of Thessalonica in 380 AD?",
        "a": "Constantine the Great",
        "b": "Nero",
        "c": "Theodosius I",
        "d": "Augustus",
        "answer": "Theodosius I"
    },
    {
        "text": "Which nation was the first in the world to officially adopt Christianity as its state religion, around 301 AD?",
        "a": "The Roman Empire",
        "b": "Ethiopia",
        "c": "Armenia",
        "d": "Georgia",
        "answer": "Armenia"
    },
    {
        "text": "What color is internationally used to indicate the steepest and most difficult ski slopes?",
        "a": "Blue",
        "b": "Red",
        "c": "Black",
        "d": "Green",
        "answer": "Black"
    },
    {
        "text": "Which of these works was NOT created for a world expo?",
        "a": "The Eiffel Tower",
        "b": "The Atomium",
        "c": "The Space Needle",
        "d": "The Sydney Opera House",
        "answer": "The Sydney Opera House"
    },
    {
        "text": "Perfectly preserved 3,300-year-old small jars have been found in Egyptian tombs. What was inside them?",
        "a": "Honey",
        "b": "Olive oil",
        "c": "Perfumes",
        "d": "Wine",
        "answer": "Honey"
    },
    {
        "text": "In which type of literature would you find a haiku?",
        "a": "Novel",
        "b": "Drama",
        "c": "Essay",
        "d": "Poetry",
        "answer": "Poetry"
    },
    {
        "text": "Which ancient Greek physician's name is used to the oath for ethics taken by doctors?",
        "a": "Hippocratic Oath",
        "b": "Agnodic Oath",
        "c": "Galen Oath",
        "d": "Aegimus Oath",
        "answer": "Hippocratic Oath"
    },
    {
        "text": "Who is the 'Iron Lady'?",
        "a": "Angela Merkel",
        "b": "Margaret Thatcher",
        "c": "Hilary Clinton",
        "d": "Alexandra Ocasio-Cortez",
        "answer": "Margaret Thatcher"
    },
    {
        "text": "What does 'http' stand for?",
        "a": "Hyperreal Transform Protocol",
        "b": "HyperText Transformation Protocol",
        "c": "Helptext Transfer Protocol",
        "d": "HyperText Transfer Protocol",
        "answer": "HyperText Transfer Protocol"
    },
    {
        "text": "Where was Che Guevara born?",
        "a": "Argentina",
        "b": "Cuba",
        "c": "Mexico",
        "d": "Bolivia",
        "answer": "Argentina"
    },
    {
        "text": "Kongō Gumi, founded in Japan in 578 AD, is the oldest still operating company in the world. What is its primary business?",
        "a": "Hotel and inn management",
        "b": "Sake brewing",
        "c": "Temple and shrine construction",
        "d": "Traditional ceremonial paper goods",
        "answer": "Temple and shrine construction"
    },
    {
        "text": "According to the TomTom Traffic Index 2025 ranking, which is the most congested city in the world?",
        "a": "Bengaluru, India",
        "b": "Mexico City, Mexico",
        "c": "Dublin, Ireland",
        "d": "Lodz, Poland",
        "answer": "Mexico City, Mexico"
    },
    {
        "text": "According to the ranking by great-circle distance, which of the following was one of the longest non-stop passenger flights in the world as of 2025?",
        "a": "Shenzhen (China) to Mexico City (Mexico)",
        "b": "New York (USA) to Singapore (Singapore)",
        "c": "Auckland (New Zealand) to Doha (Qatar)",
        "d": "Perth (Australia) to London (United Kingdom)",
        "answer": "New York (USA) to Singapore (Singapore)"
    },
    {
        "text": "Which building holds the world record as the heaviest structure, with an estimated weight of over 4 million tons?",
        "a": "The Pentagon (United States)",
        "b": "The Great Pyramid of Giza (Egypt)",
        "c": "The Palace of the Parliament (Romania)",
        "d": "The Burj Khalifa (United Arab Emirates)",
        "answer": "The Palace of the Parliament (Romania)"
    },
    {
    "text": "Which of the following creatures actually existed in real life?",
    "a": "The Griffin",
    "b": "The Dodo",
    "c": "Pegasus",
    "d": "The Phoenix",
    "answer": "The Dodo"
},
{
    "text": "Who can have type Q blood?",
    "a": "A special group of monks in Tibet",
    "b": "A human with a rare genetic mutation",
    "c": "A horse",
    "d": "It doesn't exist",
    "answer": "A horse"
},
{
    "text": "According to ancient legend, how did the Greek playwright Aeschylus die?",
    "a": "He was killed in battle",
    "b": "An eagle dropped a tortoise on his head",
    "c": "He killed himself drinking poison",
    "d": "He died from laughing",
    "answer": "An eagle dropped a tortoise on his head"
},
{
    "text": "According to the World Happiness Report 2026, which country is ranked as the happiest in the world for six years in a row?",
    "a": "Denmark",
    "b": "Iceland",
    "c": "Finland",
    "d": "Switzerland",
    "answer": "Finland"
},
{
    "text": "Which planet is, on average, the closest to Earth?",
    "a": "Venus",
    "b": "Mars",
    "c": "Mercury",
    "d": "Jupiter",
    "answer": "Mercury"
},
{
    "text": "Which is the only planet in our solar system that spins clockwise (retrograde rotation) relative to most other planets?",
    "a": "Mars",
    "b": "Venus",
    "c": "Uranus",
    "d": "Neptune",
    "answer": "Venus"
},
{
    "text": "What is the IP address commonly known as 'localhost'?",
    "a": "192.168.1.1",
    "b": "127.0.0.1",
    "c": "255.255.255.0",
    "d": "0.0.0.0",
    "answer": "127.0.0.1"
},
{
    "text": "How many bits are there in a MB?",
    "a": "1,000,000 bits",
    "b": "8,000,000 bits",
    "c": "1,048,576 bits",
    "d": "8,388,608 bits",
    "answer": "8,000,000 bits"
},
{
    "text": "Which of these programming languages is NOT typically compiled?",
    "a": "Ruby",
    "b": "Rust",
    "c": "Go",
    "d": "Java",
    "answer": "Ruby"
},
{
    "text": "What did Linus Benedict Torvalds invent?",
    "a": "The Git software",
    "b": "The Python programming language",
    "c": "The Apache web server",
    "d": "The MySQL database",
    "answer": "The Git software"
},
{
    "text": "What does BIOS stand for?",
    "a": "Basic Integrated Operating System",
    "b": "Binary Input Output System",
    "c": "Basic Input Output System",
    "d": "Built-In Operating Software",
    "answer": "Basic Input Output System"
},
{
  "text": "Which of the following is NOT a character encoding standard?",
    "a": "UTF-8",
    "b": "ANSI",
    "c": "ISO",
    "d": "ASCII",
    "answer": "ISO"
},
{
    "text": "According to Greek mythology, which evil remained inside Pandora's box after she opened it?",
    "a": "Old age",
    "b": "Sickness",
    "c": "Hope",
    "d": "Hatred",
    "answer": "Hope"
},
{
    "text": "What was probably the first man-made object capable of moving faster than the speed of sound?",
    "a": "The Bell X-1 rocket plane",
    "b": "A bullwhip",
    "c": "A bullet fired from a rifle",
    "d": "The ThrustSSC supersonic car",
    "answer": "A bullwhip"
},
{
    "text": "What do the 12 stars on the European flag represent?",
    "a": "The 12 founding member states of the European Union",
    "b": "The 12 months of the year",
    "c": "Perfection, completeness, unity, solidarity and harmony among the peoples of Europe",
    "d": "The 12 brightest stars in the European night sky",
    "answer": "Perfection, completeness, unity, solidarity and harmony among the peoples of Europe"
},
{
    "text": "Which of these songs was NOT recorded by The Beatles?",
    "a": "Hey Jude",
    "b": "Let It Be",
    "c": "Imagine",
    "d": "Yesterday",
    "answer": "Imagine"
},
{
    "text": "What is the name of the Olympic equestrian discipline in which horses perform graceful, choreographed movements often compared to ballet?",
    "a": "Show jumping",
    "b": "Eventing",
    "c": "Dressage",
    "d": "Cross-country",
    "answer": "Dressage"
},
{
    "text": "In which country was the video game Minecraft originally created?",
    "a": "Japan",
    "b": "Sweden",
    "c": "United States",
    "d": "Canada",
    "answer": "Sweden"
},
{
    "text": "The iconic Tetris theme music (often called \"Type A\") is based on a traditional song. Which one is it?",
    "a": "Frère Jacques (French nursery rhyme)",
    "b": "Korobeiniki (Russian folk song)",
    "c": "Greensleeves (English folk song)",
    "d": "Beethoven's Moonlight Sonata (Classical music)",
    "answer": "Korobeiniki (Russian folk song)"
},
{
    "text": "What is the name of the iconic masked killer in the Halloween film saga?",
    "a": "Jason Voorhees",
    "b": "Freddy Krueger",
    "c": "Michael Myers",
    "d": "Leatherface",
    "answer": "Michael Myers"
},
{
    "text": "The name of Goku's signature attack, the Kamehameha, was inspired by which of the following?",
    "a": "A type of Japanese martial arts stance",
    "b": "The sound of a crashing wave",
    "c": "A historical Hawaiian king",
    "d": "An ancient Chinese legend",
    "answer": "A historical Hawaiian king"
},
{
    "text": "What was the original profession of the video game character Super Mario?",
    "a": "Plumber",
    "b": "Carpenter",
    "c": "Chef",
    "d": "Mechanic",
    "answer": "Carpenter"
},
{
    "text": "What is the Ouroboros a symbol of?",
    "a": "A dragon eating a lion",
    "b": "A serpent or dragon eating its own tail",
    "c": "An eagle holding a snake in its beak",
    "d": "Two snakes intertwined around a winged staff",
    "answer": "A serpent or dragon eating its own tail"
},
{
    "text": "Which is the coldest capital city in the world based on average annual temperature?",
    "a": "Moscow, Russia",
    "b": "Kathmandu, Nepal",
    "c": "Ulaanbaatar, Mongolia",
    "d": "Ottawa, Canada",
    "answer": "Ulaanbaatar, Mongolia"
},
{
    "text": "What is the largest active volcano in the world?",
    "a": "Mount Fagradalsfjall",
    "b": "Mount Etna",
    "c": "Mauna Loa",
    "d": "Mount Fuji",
    "answer": "Mauna Loa"
},
{
    "text": "Which of these ingredients is NOT part of a traditional mojito cocktail recipe?",
    "a": "Cane sugar",
    "b": "Mint leaves",
    "c": "Gin",
    "d": "Lime juice",
    "answer": "Gin"
},
{
    "text": "Which gas, ofetn used as an anesthetic by dentists, is commonly known as 'laughing gas'?",
    "a": "Sulphur dioxide",
    "b": "Carbon monoxide",
    "c":  "Hydrogen peroxide",
    "d": "Nitrous oxide",
    "answer": "Nitrous oxide"
},
{
    "text": "By which international agreement was the Kyoto Protocol effectively replaced?",
    "a": "The Copenhagen Accord",
    "b": "The Paris Agreement",
    "c": "The Montreal Protocol",
    "d": "The Rio Declaration",
    "answer": "The Paris Agreement"
},
{
    "text": "Who was Anna Maria Tussaud?",
    "a": "A famous opera singer from Vienna",
    "b": "A French sculptress from the 18th century",
    "c": "A queen of England and fashion icon",
    "d": "A British novelist who wrote under a pseudonym",
    "answer": "A French sculptress from the 18th century"
},
{
    "text": "What is jamais vu?",
    "a": "The feeling of seeing something unfamiliar as if you've seen it before",
    "b": "A type of French pastry filled with cream",
    "c": "The sensation that a familiar place or situation seems completely new or unreal",
    "d": "A vintage French film genre from the 1950s",
    "answer": "The sensation that a familiar place or situation seems completely new or unreal"
},
{
    "text": "Which of these countries is NOT an enclave (a state entirely surrounded by another state)?",
    "a": "San Marino",
    "b": "Lesotho",
    "c": "Liechtenstein",
    "d": "Vatican City",
    "answer": "Liechtenstein"
},
{
    "text": "Which of these islands or landmasses was discovered most recently by Europeans?",
    "a": "Australia",
    "b": "Greenland",
    "c": "Hawaii",
    "d": "Antarctica",
    "answer": "Antarctica"
},
{
    "text": "What does the abbreviation CV stand for in the context of job applications?",
    "a": "Curriculum Vitae",
    "b": "Career Verification",
    "c": "Covered Vacancy",
    "d": "Certificate of Validation",
    "answer": "Curriculum Vitae"
},
{
    "text": "According to medical experts who analyzed the series, which physical detail about Dr. House is medically incorrect?",
    "a": "He uses his cane on the wrong side",
    "b": "He takes Vicodin at the wrong time of day",
    "c": "His leg scar is not deep enough",
    "d": "He has no limp when walking without his cane",
    "answer": "He uses his cane on the wrong side"
},
{
    "text": "Where is the original bronze statue of The Thinker (Le Penseur) located?",
    "a": "At the Rodin Museum in Paris, France",
    "b": "Inside the Panthéon in Paris, France",
    "c": "At the Louvre Museum in Paris, France",
    "d": "At the Vatican Museums, Vatican City",
    "answer": "At the Rodin Museum in Paris, France"
},
{
    "text": "What is the name of the world-famous beach in Rio de Janeiro, Brazil?",
    "a": "Ipanema",
    "b": "Copacabana",
    "c": "Maracanã",
    "d": "Botafogo",
    "answer": "Copacabana"
},
{
    "text": "Who currently holds the world record for the most-attended ticketed single concert by an artist?",
    "a": "Lady Gaga (Rio de Janeiro, 2025)",
    "b": "Vasco Rossi (Modena, 2017)",
    "c": "Marko Perković (Zagreb, 2025)",
    "d": "Travis Scott (Delhi, 2025)",
    "answer": "Marko Perković (Zagreb, 2025)"
},
{
    "text": "Between 1954 and 1991, the KGB was the state security service police of which country?",
    "a": "China",
    "b": "East Germany",
    "c": "Soviet Union",
    "d": "North Korea",
    "answer": "Soviet Union"
},
{
    "text": "What is surströmming, considered the smelliest food in the world?",
    "a": "A Swedish fermented herring",
    "b": "A Japanese natto dish",
    "c": "An Icelandic fermented shark",
    "d": "A Chinese old rotten egg",
    "answer": "A Swedish fermented herring"
},
{
    "text": "Excluding its pedestal, which statue is the tallest in the world with 182m?",
    "a": "Spring Temple Buddha in China",
    "b": "Laykyun Sekkya in Myanmar",
    "c": "Statue of Unity in India",
    "d": "Statue of Liberty in USA",
    "answer": "Statue of Unity in India"
},
{
    "text": "What is depicted by the smallest handmade sculpture in history, measuring only 0.078 mm (78 microns) in length and crafted from a carpet fiber?",
    "a": "A motorcycle made from 24-carat gold",
    "b": "A human embryo (or foetus)",
    "c": "William Shakespeare",
    "d": "A polar bear",
    "answer": "A human embryo (or foetus)"
},
{
    "text": "What is Aibohphobia?",
    "a": "The fear of heights",
    "b": "The fear of palindromes",
    "c": "The fear of needles",
    "d": "The fear of long words",
    "answer": "The fear of palindromes"
},
{
    "text": "What is a geometric solid with twenty faces called?",
    "a": "Dodecahedron",
    "b": "Icosahedron",
    "c": "Octahedron",
    "d": "Tetrahedron",
    "answer": "Icosahedron"
},
{
    "text": "How many edges does a pentagonal pyramid have?",
    "a": "5",
    "b": "6",
    "c": "8",
    "d": "10",
    "answer": "10"
},
{
    "text": "How many edges does a solid formed by two square-based pyramids joined at their bases have?",
    "a": "8",
    "b": "10",
    "c": "12",
    "d": "16",
    "answer": "12"
},
{
    "text": "Where is the Sea of Tranquility (Mare Tranquillitatis) located?",
    "a": "In the Pacific Ocean",
    "b": "In the Caribbean Sea",
    "c": "On the Moon",
    "d": "In the Mediterranean Sea",
    "answer": "On the Moon"
},
{
    "text": "What is the name of the deepest hole humanity has ever dug, with a true vertical depth of 12,262 meters?",
    "a": "IceCube Neutrino Observatory (Antarctica)",
    "b": "Bingham Canyon Mine (USA)",
    "c": "Kola Superdeep Borehole (Russia)",
    "d": "TauTona Mine (South Africa)",
    "answer": "Kola Superdeep Borehole (Russia)"
},
{
    "text": "In Jonathan Swift's *Gulliver's Travels*, when Gulliver is shipwrecked in Brobdingnag, whom does he encounter?",
    "a": "A race of tiny people",
    "b": "A race of giants",
    "c": "Intelligent horses",
    "d": "Floating island inhabitants",
    "answer": "A race of giants"
},
{
    "text": "Which of the following is NOT a virtual assistant?",
    "a": "Alexa",
    "b": "Cortana",
    "c": "Bixby",
    "d": "Oracle",
    "answer": "Oracle"
},
{
    "text": "Which country won the first FIFA World Cup in 1930?",
    "a": "Brazil",
    "b": "Italy",
    "c": "England",
    "d": "Uruguay",
    "answer": "Uruguay"
},
{
    "text": "What was the original name of New York before it was renamed by the British?",
    "a": "Nieuw Amsterdam",
    "b": "New Angoulême",
    "c": "Nueva York",
    "d": "Fort Orange",
    "answer": "Nieuw Amsterdam"
},
{
    "text": "What does the word \"Mamihlapinatapai\" from the Yahgan language mean?",
    "a": "The feeling of being completely exhausted after a long journey",
    "b": "A look shared between two people, each hoping the other will initiate something neither wants to start",
    "c": "The sound of waves crashing against the shore at night",
    "d": "A deep and unbreakable friendship between warriors",
    "answer": "A look shared between two people, each hoping the other will initiate something neither wants to start"
},
{
    "text": "How many possible opening moves are there for a player in a game of chess?",
    "a": "8",
    "b": "12",
    "c": "16",
    "d": "20",
    "answer": "20"
},
{
    "text": "Which animal is often considered to have the highest intelligence (or \"IQ\") after humans?",
    "a": "African Grey Parrot",
    "b": "Chimpanzee",
    "c": "Bottlenose Dolphin",
    "d": "Elephant",
    "answer": "Chimpanzee"
},
{
    "text": "On the google.com homepage, the two letter Gs in the standard Google logo are usually what color?",
    "a": "Blue",
    "b": "Yellow",
    "c": "Green",
    "d": "Red",
    "answer": "Blue"
},
{
    "text": "What was the first known name of the city now called Istanbul?",
    "a": "Byzantium",
    "b": "Constantinople",
    "c": "Lygos",
    "d": "New Rome",
    "answer": "Lygos"
},
{
    "text": "Wagyu is a Japanese breed of which animal?",
    "a": "Cow",
    "b": "Sheep",
    "c": "Pig",
    "d": "Chicken",
    "answer": "Cow"
},
{
    "text": "In the famous manga and anime *Death Note*, what is the Shinigami Ryuk particularly fond of eating?",
    "a": "Human souls",
    "b": "Chocolate",
    "c": "Apples",
    "d": "Insects",
    "answer": "Apples"
},
{
    "text": "The city of Dubai is located on which body of water?",
    "a": "Red Sea",
    "b": "Bosporus",
    "c": "Arabian Sea",
    "d": "Persian Gulf",
    "answer": "Persian Gulf"   
},
{
    "text": "Which coin designed in 1879 is the oldest still in circulation and remains legal tender today?",
    "a": "The US Penny",
    "b": "The British Pound",
    "c": "The Swiss 10-centime coin",
    "d": "The Japanese 1-yen coin",
    "answer": "The Swiss 10-centime coin"
},
{
    "text": "The iconic Disney intro jingle (or fanfare) that accompanies the castle logo is taken from which movie?",
    "a": "Snow White and the Seven Dwarfs",
    "b": "Pinocchio",
    "c": "Fantasia",
    "d": "Cinderella",
    "answer": "Pinocchio"
},
{
    "text": "Which of these films is a completely silent movie?",
    "a": "The Wizard of Oz",
    "b": "Modern Times",
    "c": "Metropolis",
    "d": "Citizen Kane",
    "answer": "Metropolis"
},
{
    "text": "The Airbus A380-800 is the world's largest commercial passenger airliner. How many passengers can it carry in a single-class configuration?",
    "a": "525",
    "b": "755",
    "c": "853",
    "d": "993",
    "answer": "853"
},
{
  "text": "Which gemstone holds the record for the highest price ever paid at auction, selling for $71.2 million in 2017?",
  "a": "A pink diamond",
  "b": "Jadeite",
  "c": "A Marie Antoinette pearl",
  "d": "A ruby",
  "answer": "A pink diamond"
},
{
    "text": "When did construction of the Berlin Wall begin?",
    "a": "1949",
    "b": "1961",
    "c": "1974",
    "d": "1989",
    "answer": "1961"
},
{
    "text": "Which scale, named after a German mineralogist, is used to measure the hardness of minerals?",
    "a": "Mohs scale",
    "b": "Beaufort scale",
    "c": "pH scale",
    "d": "Richter scale",
    "answer": "Mohs scale"
},
{
    "text": "What is the name of the syndrome characterized by fainting and confusion in individuals exposed to an overwhelming concentration of artistic beauty?",
    "a": "Stockholm syndrome",
    "b": "Stendhal syndrome",
    "c": "Paris syndrome",
    "d": "Münchhausen syndrome",
    "answer": "Stendhal syndrome"
},
{
    "text": "Which of these pop, literary or historical figures best represents the Dunning-Kruger effect?",
    "a": "Don Quixote",
    "b": "Sisyphus",
    "c": "Bugs Bunny",
    "d": "Faust",
    "answer": "Don Quixote"
},
{
    "text": "The main characters of *Rick and Morty* are directly inspired by which classic film?",
    "a": "Back to the Future",
    "b": "Ghostbusters",
    "c": "The Adventures of Buckaroo Banzai",
    "d": "Star Wars",
    "answer": "Back to the Future"
},
{
    "text": "What was the only word repeatedly spoken by the raven in Edgar Allan Poe's famous poem?",
    "a": "Nevermore",
    "b": "Forever",
    "c": "Alone",
    "d": "Lost",
    "answer": "Nevermore"
},
{
    "text": "Which of these is NOT one of the New7Wonders of the World?",
    "a": "The Great Wall of China",
    "b": "Chichen Itza",
    "c": "The Statue of Liberty",
    "d": "Machu Picchu",
    "answer": "The Statue of Liberty"
},
{
    "text": "The programming language originally named Oak was renamed Java due to copyright issues. What inspired the new name?",
    "a": "A type of coffee",
    "b": "An island",
    "c": "A sea",
    "d": "A species of bird",
    "answer": "A type of coffee",
},
{
    "text": "Which of these logical operators has the highest precedence in the C programming language?",
    "a": "||",
    "b": "&&",
    "c": "==",
    "d": "!",
    "answer": "!"
},
{
    "text": "How long does it take for Earth to complete one orbit around the Sun?",
    "a": "31 days",
    "b": "1 years",
    "c": "365 years",
    "d": "1 day",
    "answer": "1 year"
},
{
    "text": "Which of the following is the LARGEST in number?",
    "a": "Cells in the human body",
    "b": "Stars in the Milky Way",
    "c": "Insects on Earth",
    "d": "Atoms in a single grain of sand",
    "answer": "Atoms in a single grain of sand"
},
{
    "text": "How many balls are used in a standard game of billiards (specifically pool or eight-ball)?",
    "a": "11",
    "b": "15",
    "c": "16",
    "d": "19",
    "answer": "16"
},
{
    "text": "Who was the sculptor of the Statue of Liberty?",
    "a": "Frédéric-Auguste Bartholdi",
    "b": "Alexandre-Gustave Eiffel",
    "c": "Richard Morris Hunt",
    "d": "Eugène Viollet-le-Duc",
    "answer": "Frédéric-Auguste Bartholdi"
},
{
    "text": "Approximately how many times does blood circulate through the entire body in one day?",
    "a": "500–600 times",
    "b": "1,000–1,500 times",
    "c": "2,000–2,500 times",
    "d": "3,000–4,000 times",
    "answer": "1,000–1,500 times"
},
{
    "text": "What is the real name of Santa Claus, based on the historical figure who inspired the legend?",
    "a": "Kris Kringle",
    "b": "Saint Nicholas",
    "c": "Father Christmas",
    "d": "Sinterklaas",
    "answer": "Saint Nicholas"
},
{
    "text": "What is \"Stan\", the unofficial mascot of Google?",
    "a": "A blue rolling robot",
    "b": "A Tyrannosaurus rex",
    "c": "A pink flamingo",
    "d": "A squirrel",
    "answer": "A Tyrannosaurus rex"
},
{
    "text": "What does the word \"dinosaur\" mean?",
    "a": "Great lizard",
    "b": "Thunder lizard",
    "c": "Terrible lizard",
    "d": "Ancient reptile",
    "answer": "Terrible lizard"
},
{
    "text": "During which geological era did dinosaurs live?",
    "a": "Paleozoic Era",
    "b": "Mesozoic Era",
    "c": "Cenozoic Era",
    "d": "Proterozoic Era",
    "answer": "Mesozoic Era"
},
{
    "text": "At the end of which geological period did the extinction of the non-avian dinosaurs occur?",
    "a": "Triassic",
    "b": "Cretaceous",
    "c": "Permian",
    "d": "Jurassic",
    "answer": "Cretaceous"
},
{
    "text": "Which of these holidays occurs later in the calendar year?",
    "a": "Earth Day",
    "b": "Saint Patrick's Day",
    "c": "International Women's Day",
    "d": "April Fools' Day",
    "answer": "Earth Day"
},
{
    "text": "Where does the name \"Scooby-Doo\" come from?",
    "a": "From a Frank Sinatra song",
    "b": "From a famous dog in a children's book",
    "c": "From the name of a cartoonist's childhood pet",
    "d": "From a brand of dog food",
    "answer": "From a Frank Sinatra song"
},
{
    "text": "What shape is the iconic Pringles potato crisp?",
    "a": "An elliptic paraboloid",
    "b": "A hyperbolic paraboloid",
    "c": "A hyperboloid of one sheet",
    "d": "A saddle-shaped parabolic cylinder",
    "answer": "A hyperbolic paraboloid"
},
{
    "text": "Which swimming style is generally considered the second fastest after freestyle (front crawl) in competitive swimming?",
    "a": "Butterfly",
    "b": "Backstroke",
    "c": "Breaststroke",
    "d": "Sidestroke",
    "answer": "Butterfly"
},
{
    "text": "What is the name of the ballet pose in which one leg is raised en air (extended) behind the body, and the arms are stretched in opposite directions?",
    "a": "Arabesque",
    "b": "Plié",
    "c": "Dégagé",
    "d": "Développé",
    "answer": "Arabesque"
},
{
    "text": "What was Operation Danube?",
    "a": "The Soviet-led invasion of Czechoslovakia in 1968 to suppress the Prague Spring",
    "b": "A NATO military exercise conducted in West Germany during the Cold War",
    "c": "A joint US-Soviet space mission in the 1970s",
    "d": "The construction project of the Danube River dam in Romania",
    "answer": "The code name for the Soviet-led invasion of Czechoslovakia in 1968 to suppress the Prague Spring"
},
{
    "text": "What is the name of the famous book containing the quotations and speeches of Mao Zedong?",
    "a": "The Red Manifesto",
    "b": "The Great Leap Forward",
    "c": "The Little Red Book",
    "d": "The Cultural Revolution Manual",
    "answer": "The Little Red Book"
},
{
    "text": "When were the first Winter Olympics held in Chamonix, France?",
    "a": "1914",
    "b": "1924",
    "c": "1942",
    "d": "1958",
    "answer": "1924"
},
{
    "text": "What is the name of the Japanese ritual suicide practiced by samurai, which originated in the 12th century?",
    "a": "Kamikaze",
    "b": "Seppuku",
    "c": "Shinju",
    "d": "Junshi",
    "answer": "Seppuku"
},
{
    "text": "The term \"Internet meme\" was coined by American attorney Mike Godwin in 1993, in reference to the original word \"meme,\" which was coined by Richard Dawkins. What is Richard Dawkins' profession?",
    "a": "Philosopher",
    "b": "Evolutionary biologist",
    "c": "Linguist",
    "d": "Sociologist",
    "answer": "Evolutionary biologist"
},
{
    "text": "The term \"meme\" derives from the ancient Greek word mīmēma. What does it mean?",
    "a": "Imitated thing",
    "b": "Laughter or joke",
    "c": "Idea or thought",
    "d": "Symbol or sign",
    "answer": "Imitated thing"
},
{
    "text": "Why did Pelé never win the Ballon d'Or during his active career?",
    "a": "The award was only open to European players until 1995",
    "b": "Because he never played for a European club",
    "c": "He was nominated but never won the final vote",
    "d": "He was disqualified due to his involvement in political activities",
    "answer": "The award was only open to European players until 1995"
},
{
    "text": "What is the name and appearance of the flag used in Formula 1 to signal the end of a race?",
    "a": "A solid red flag",
    "b": "A black and white checkered flag",
    "c": "A blue flag with a yellow stripe",
    "d": "A green flag with a white cross",
    "answer": "A black and white checkered flag"
},
{
    "text": "Augusto Pinochet ruled Chile as a dictator from 1973 until 1990. How did he eventually leave power?",
    "a": "He was overthrown by a popular uprising",
    "b": "He was assassinated in a military coup",
    "c": "He lost a plebiscite and stepped down, leading to democratic elections",
    "d": "He was arrested and convicted by the International Criminal Court",
    "answer": "He lost a plebiscite and stepped down, leading to democratic elections"
},
{
    "text": "What is the name of the chief of medicine at Sacred Heart Hospital in the early seasons of the TV series *Scrubs*?",
    "a": "Dr. Bob Kelso",
    "b": "Dr. Perry Cox",
    "c": "Dr. J.D.",
    "d": "Dr. Townshend",
    "answer": "Dr. Bob Kelso"
},
{
    "text": "What was considered one of the main causes of the depletion of the ozone layer (ozone hole)?",
    "a": "Carbon dioxide (CO2) emissions from cars",
    "b": "Chlorofluorocarbons (CFCs) used in aerosols and refrigerants",
    "c": "Methane emissions from livestock",
    "d": "Sulfur dioxide from volcanic eruptions",
    "answer": "Chlorofluorocarbons (CFCs) used in aerosols and refrigerants"
},
{
    "text": "In which winter sport did Steven Bradbury win a gold medal at the 2002 Winter Olympics in Salt Lake City?",
    "a": "Alpine skiing",
    "b": "Figure skating",
    "c": "Short track speed skating",
    "d": "Cross-country skiing",
    "answer": "Short track speed skating"
},
{
    "text": "It is possible to form a grammatically correct English sentence by repeating which word several times in a row?",
    "a": "Buffalo",
    "b": "Duck",
    "c": "Mouse",
    "d": "Cat",
    "answer": "Buffalo"
},
{
    "text": "What is the name of the character created by Jules Verne who travels 'Around the World in Eighty Days'?",
    "a": "Otto Lidenbrock",
    "b": "Phileas Fogg",
    "c": "Huckleberry Finn",
    "d": "Uriah Heep",
    "answer": "Phileas Fogg"
},
{
   "text": "What is the phenomenon whereby a tiny localised change can have large effects elsewhere?",
   "a": "Boomerang Effect",
   "b": "Butterfly Effect",
   "c": "Doppler Effect",
   "d": "Generation Effect",
   "answer": "Butterfly Effect"
},
{
    "text": "Which of these international dishes is NOT also the name of the cooking vessel or pan used to prepare it?",
    "a": "Paella",
    "b": "Tajine",
    "c": "Wok",
    "d": "Ramen",
    "answer": "Ramen"
},
{
    "text": "Tasmania is an island state of which country?",
    "a": "Indonesia",
    "b": "Mozambique",
    "c": "Kenya",
    "d": "Australia",
    "answer": "Australia"
},
{
    "text": "'3 May. Bistritz. Left Munich at 8:35 P.M.' are the opening words to which novel?",
    "a": "Tinker Tailor Soldier Spy",
    "b": "Dracula",
    "c": "Heart of Darkness",
    "d": "Frankenstein",
    "answer": "Dracula"
},
{
    "text": "Which of these video games is played exclusively from a third-person perspective?",
    "a": "Elden Ring",
    "b": "Call of Duty",
    "c": "Counter-Strike",
    "d": "DOOM",
    "answer": "Elden Ring"
},
{
    "text": "What is the name of the casino employee who operates the roulette wheel and manages bets?",
    "a": "Dealer",
    "b": "Croupier",
    "c": "Pit boss",
    "d": "Stickman",
    "answer": "Croupier"
},
{
    "text": "What is the sum of the five interior angles of a pentagon?",
    "a": "360°",
    "b": "540°",
    "c": "660°",
    "d": "720°",
    "answer": "540°"
},
{
    "text": "Which is the most spoken Chinese dialect?",
    "a": "Cantonese",
    "b": "Wu",
    "c": "Mandarin",
    "d": "Min",
    "answer": "Mandarin"
},
{
    "text": "Which guitar model was most famously associated with Jimi Hendrix?",
    "a": "Gibson Les Paul",
    "b": "Fender Telecaster",
    "c": "Fender Stratocaster",
    "d": "Gibson SG",
    "answer": "Fender Stratocaster"
},
{
    "text": "What does Dorian Gray wish for in front of his portrait in Oscar Wilde's novel *The Picture of Dorian Gray*?",
    "a": "That he could remain young forever",
    "b": "That he could become a famous painter",
    "c": "That he could gain wealth and social power",
    "d": "That his soul would be purified",
    "answer": "That he could remain young forever"
},
{
    "text": "Which artistic movement, characterized by sinuous lines, nature motifs, and decorative elegance, reached its peak during the Belle Époque?",
    "a": "Art Nouveau",
    "b": "Baroque",
    "c": "Art Deco",
    "d": "Rococo",
    "answer": "Art Nouveau"
},
{
    "text": "Who directed the 1973 film *Amarcord*?",
    "a": "Luchino Visconti",
    "b": "Michelangelo Antonioni",
    "c": "Federico Fellini",
    "d": "Pier Paolo Pasolini",
    "answer": "Federico Fellini"
},
{
    "text": "What is the name of the actor who originally portrayed Spock in the *Star Trek* original series and later reprised the role in the *Star Trek* reboot films?",
    "a": "William Shatner",
    "b": "Leonard Nimoy",
    "c": "Zachary Quinto",
    "d": "Patrick Stewart",
    "answer": "Leonard Nimoy"
},
{
    "text": "What is the title of the 1929 short film by Luis Buñuel and Salvador Dalí, considered the most significant work of surrealist cinema?",
    "a": "The Andalusian Dog (Un Chien Andalou)",
    "b": "The Golden Age (L'Âge d'Or)",
    "c": "Land Without Bread (Las Hurdes)",
    "d": "The Phantom of Liberty (Le Fantôme de la liberté)",
    "answer": "The Andalusian Dog (Un Chien Andalou)"
},
{
    "text": "Canadair aircraft are primarily used for what specific purpose?",
    "a": "Military transport",
    "b": "Aerial firefighting",
    "c": "Commercial passenger flights",
    "d": "Weather reconnaissance",
    "answer": "Aerial firefighting"
},
{
    "text": "Who created the Grinch?",
    "a": "Roald Dahl",
    "b": "Walt Disney",
    "c": "Dr. Seuss",
    "d": "Charles Schulz",
    "answer": "Dr. Seuss"
},
{
    "text": "What is the name of the collective dance often performed at punk, hardcore, and rock concerts, characterized by energetic jumping up and down?",
    "a": "Moshing",
    "b": "Headbanging",
    "c": "Pogo",
    "d": "Circle pit",
    "answer": "Pogo"
},
{
    "text": "What is the traditional filling of the Sachertorte, the famous Viennese chocolate cake invented in 1832?",
    "a": "Whipped cream",
    "b": "Vanilla custard",
    "c": "Apricot jam",
    "d": "Raspberry purée",
    "answer": "Apricot jam"
},
{
    "text": "Which ear did Vincent van Gogh famously cut off?",
    "a": "His right ear",
    "b": "His left ear",
    "c": "Both ears",
    "d": "None",
    "answer": "His left ear"
},
{
    "text": "Ferdinand Lewis Alcindor Jr. is the real name of which famous athlete?",
    "a": "Muhammad Ali",
    "b": "Kareem Abdul-Jabbar",
    "c": "Shaquille O'Neal",
    "d": "LeBron James",
    "answer": "Kareem Abdul-Jabbar"
},
{
    "text": "Which airport offers flights to the largest number of different countries worldwide?",
    "a": "Dubai International Airport (DXB), UAE",
    "b": "Paris-Charles de Gaulle Airport (CDG), France",
    "c": "Istanbul Airport (IST), Turkey",
    "d": "Frankfurt Airport (FRA), Germany",
    "answer": "Istanbul Airport (IST), Turkey"
},
{
    "text": "Which metal must be added to copper to obtain bronze?",
    "a": "Iron",
    "b": "Lead",
    "c": "Zinc",
    "d": "Tin",
    "answer": "Tin"
},
{
    "text": "Which of these names does not appear in the standard NATO phonetic alphabet adopted in 1956?",
    "a": "Mike",
    "b": "Charlie",
    "c": "Roger",
    "d": "Victor",
    "answer": "Roger"
},
{
    "text": "What is the national animal of Scotland?",
    "a": "Unicorn",
    "b": "Dragon",
    "c": "Griffin",
    "d": "Red deer",
    "answer": "Unicorn"
},
{
    "text": "What is the first color in the official order of the Olympic rings when described from left to right?",
    "a": "Blue",
    "b": "Black",
    "c": "Yellow",
    "d": "Green",
    "answer": "Blue"
},
{
    "text": "What is the smallest bone in the human body?",
    "a": "Stapes (in the middle ear)",
    "b": "Patella (kneecap)",
    "c": "Phalanx (finger bone)",
    "d": "Carpal (wrist bone)",
    "answer": "Stapes (in the middle ear)"
},
{
  "text": "What is shoefiti?",
  "a": "A practice of throwing sneakers over power lines",
  "b": "A type of traditional Japanese footwear",
  "c": "A social media trend about collecting rare sneakers",
  "d": "A cleaning technique for restoring old shoes",
  "answer": "A practice of throwing sneakers over power lines"
},
{
    "text": "Which city's metro system has the highest number of unique stations, with 424 stations?",
    "a": "Shanghai, China",
    "b": "Seoul, South Korea",
    "c": "Paris, France",
    "d": "New York, United States",
    "answer": "New York, United States"
},
{
    "text": "If you were lost at sea, which of the following would you use to ask for help?",
    "a": "... --- ...",
    "b": "... .. -- ---",
    "c": "...-",
    "d": ".-. .- - ...",
    "answer": "... --- ..."
},
{
    "text": "Which of the following concepts is most closely associated with the philosophy of Søren Kierkegaard?",
    "a": "The will to power",
    "b": "The leap of faith",
    "c": "The categorical imperative",
    "d": "The death of God",
    "answer": "The leap of faith"
},
{
    "text": "Which of these is the most banal?",
    "a": "The power",
    "b": "The faith",
    "c": "The evil",
    "d": "The imperative",
    "answer": "The evil"
},
{
    "text": "Which country has the most islands in the world?",
    "a": "Sweden",
    "b": "Japan",
    "c": "Philippines",
    "d": "France",
    "answer": "Sweden"
},
{
    "text": "In 1992, engineer Neil Papworth sent the first SMS text message. What was the content of that message?",
    "a": "Hello World",
    "b": "Merry Christmas",
    "c": "Can you hear me?",
    "d": "Test Message",
    "answer": "Merry Christmas"
},
{
    "text": "Which of these paintings was created first?",
    "a": "The Ballet Class (Degas)",
    "b": "The Starry Night (van Gogh)",
    "c": "The Great Wave off Kanagawa (Hokusai)",
    "d": "The Kiss (Hayez)",
    "answer": "The Great Wave off Kanagawa (Hokusai)"
},
{
    "text": "In which city was the workshop of Antonio Stradivari, the famous maker of string instruments, located during the 18th century?",
    "a": "Venice",
    "b": "Paris",
    "c": "Cremona",
    "d": "Naples",
    "answer": "Cremona"
},
{
    "text": "Which of these inventions was NOT available during the lifetime of Abraham Lincoln?",
    "a": "Lawnmower",
    "b": "Morse telegraph",
    "c": "Motor passenger elevator",
    "d": "Toilet paper roll",
    "answer": "Toilet paper roll"
},
{
    "text": "Born in 1887, which artist was renowned for using everyday objects to make works of art he called 'ready-mades'?",
    "a": "Marcel Duchamp",
    "b": "Georges Braque",
    "c": "Jean Cocteau",
    "d": "René Magritte",
    "answer": "Marcel Duchamp"
},
{
    "text": "Which of these best describes 'ibuprofen'",
    "a": "Antihistamine",
    "b": "Anti-inflammatory",
    "c": "Antidepressant",
    "d": "Antibiotic",
    "answer": "Anti-inflammatory"
},
{
    "text": "According to their offcial measurements, which of these paintings is the smallest?",
    "a": "The starry night",
    "b": "Mona Lisa",
    "c": "Girl with a pearl earring",
    "d": "American Gothic",
    "answer": "Girl with a pearl earring"
},
{
    "text": "What is the unique characteristic of Im Dong-hyun, the South Korean archer and multiple Olympic gold medalist and world record holder?",
    "a": "He won with a Robin Hood shot, splitting one arrow with another",
    "b": "He is legally blind",
    "c": "He has only one arm",
    "d": "He was 91 years old when he won his first gold medal",
    "answer": "He is legally blind"
},
{
    "text": "Generally, how many holes does a standard golf course consist of?",
    "a": "9",
    "b": "12",
    "c": "18",
    "d": "21",
    "answer": "18"
},
{
    "text": "Which country holds the world record for the highest number of official languages recognized at the national level?",
    "a": "Papua New Guinea",
    "b": "India",
    "c": "Bolivia",
    "d": "South Africa",
    "answer": "Bolivia"
},
{
    "text": "How many syllables does a traditional Japanese haiku consist of?",
    "a": "15",
    "b": "17",
    "c": "19",
    "d": "21",
    "answer": "17"
},
{
    "text": "Which of these works was NOT left incomplete?",
    "a": "Turandot by Puccini",
    "b": "Game of Death by Bruce Lee",
    "c": "Something's Got to Give by Marilyn Monroe",
    "d": "Resurrection by Tolstoy",
    "answer": "Resurrection by Tolstoy"
},
{
    "text": "How many badges are there in this game?",
    "a": "10",
    "b": "14",
    "c": "17",
    "d": "20",
    "answer": "20"
},
{
    "text": "Which of these cities has a fully operational metro system as of 2026?",
    "a": "Manila (Philippines)",
    "b": "Reykjavik (Iceland)",
    "c": "Amsterdam (Netherlands)",
    "d": "Bogotá (Colombia)",
    "answer": "Amsterdam (Netherlands)"
},
{
    "text": "Which of these is a NoSQL database commonly used for storing JSON-like documents?",
    "a": "MySQL",
    "b": "PostgreSQL",
    "c": "MongoDB",
    "d": "SQLite",
    "answer": "MongoDB"
},
{
    "text": "Which of the following types of memory is fastest?",
    "a": "SSD (Solid State Drive)",
    "b": "HDD (Hard Disk Drive)",
    "c": "RAM (Random Access Memory)",
    "d": "ROM (Read Only Memory)",
    "answer": "RAM (Random Access Memory)"
},
{
    "text": "Which football team was referred to as \"Los Galácticos\"?",
    "a": "FC Barcelona (2008–2012)",
    "b": "Real Madrid (2000–2006)",
    "c": "Manchester United (1998-1999)",
    "d": "AC Milan (1988-1989)",
    "answer": "Real Madrid (2000–2006)"
},
{
    "text": "What is the name of Aladdin's monkey in Disney's Aladdin?",
    "a": "Rajah",
    "b": "Iago",
    "c": "Abu",
    "d": "Jafar",
    "answer": "Abu"
},
{
    "text": "In Looney Tunes, which character is the constant rival of Bugs Bunny, often trying (and failing) to catch him?",
    "a": "Tweety Bird",
    "b": "Elmer Fudd",
    "c": "Marvin the Martian",
    "d": "Porky Pig",
    "answer": "Elmer Fudd"
},
{
    "text": "What animal is Beep Beep, the rival of Wile E. Coyote in the Looney Tunes cartoons?",
    "a": "A falcon",
    "b": "A roadrunner",
    "c": "A pheasant",
    "d": "A vulture",
    "answer": "A roadrunner"
},
{
    "text": "What did magician David Copperfield make disappear in front of a live television audience in 1983?",
    "a": "The president of the United States Ronald Reagan",
    "b": "An 8-meter-long New York City limousine",
    "c": "The Statue of Liberty",
    "d": "The Hollywood Sign",
    "answer": "The Statue of Liberty"
},
{
    "text": "In a famous 1971 experiment paying tribute to Galileo, Apollo 15 astronaut David Scott dropped a hammer and a feather on the lunar surface. Which hit the ground first?",
    "a": "The hammer",
    "b": "The feather",
    "c": "They landed at the same time",
    "d": "Neither, they floated away due to low gravity",
    "answer": "They landed at the same time"
},
{
    "text": "Who was nicknamed \"Ol' Blue Eyes\"?",
    "a": "Frank Sinatra",
    "b": "Elton John",
    "c": "Paul Anka",
    "d": "Elvis Presley",
    "answer": "Frank Sinatra"
},
{
    "text": "Who is the director of the Oscar-winning film The Last of the Mohicans (1992)?",
    "a": "Ridley Scott",
    "b": "James Cameron",
    "c": "Michael Mann",
    "d": "Oliver Stone",
    "answer": "Michael Mann"
},
{
    "text": "Which of the following languages is written from left to right (LTR)?",
    "a": "Arabic",
    "b": "Hebrew",
    "c": "Urdu",
    "d": "Malay",
    "answer": "Malay"
},
{
    "text": "What was the \"Sword of Damocles\" suspended by?",
    "a": "A single hair",
    "b": "An invisible force",
    "c": "Ariadne's thread",
    "d": "A golden rope",
    "answer": "A single hair"
},
{
    "text": "What does the acronym USB stand for?",
    "a": "Universal Serial Bus",
    "b": "United System Bus",
    "c": "Universal Synchronous Buffer",
    "d": "Unified Serial Byte",
    "answer": "Universal Serial Bus"
},
{
    "text": "Which country do the Galápagos Islands belong to?",
    "a": "Ecuador",
    "b": "Colombia",
    "c": "Peru",
    "d": "Panama",
    "answer": "Ecuador"
},
{
    "text": "Which chemical element has the symbol Ce?",
    "a": "Cesium",
    "b": "Cerium",
    "c": "Calcium",
    "d": "Cobalt",
    "answer": "Cerium"
},
{
    "text": "In 732 AD, who was the protagonist of the historic Battle of Tours (also known as the Battle of Poitiers)?",
    "a": "Charlemagne",
    "b": "Charles Martel",
    "c": "Charles the Bald",
    "d": "Charles IV the Fair",
    "answer": "Charles Martel"
},
{
    "text": "Where is Ilya Ilyich Oblomov most likely to be encountered?",
    "a": "Lying on his sofa",
    "b": "In his office",
    "c": "On his country estate",
    "d": "Traveling abroad",
    "answer": "Lying on his sofa"
},
{
    "text": "What is a caryatid?",
    "a": "A major blood vessel that supplies blood to the brain, neck, and face",
    "b": "A sculpted female figure serving as column or pillar",
    "c": "A jewel characterized by the presence of a single stone, usually a diamond or a zircon",
    "d": "The breakdown of teeth due to acids produced by bacteria",
    "answer": "A sculpted female figure serving as column or pillar"
},
{
    "text": "Which of these mountain ranges is located in South America?",
    "a": "Pyrenees",
    "b": "Andes",
    "c": "Dolomites",
    "d": "Ural Mountains",
    "answer": "Andes"
},
{
    "text": "In which year did the Kronstadt rebellion take place?",
    "a": "1917",
    "b": "1920",
    "c": "1921",
    "d": "1928",
    "answer": "1921"
},
{
    "text": "How many 'brains' and hearts does an octopus typically have?",
    "a": "4 'brains' and 6 hearts",
    "b": "9 'brains' and 3 hearts",
    "c": "6 'brains' and 4 hearts",
    "d": "1 brain and 1 rear end (butt)",
    "answer": "9 'brains' and 3 hearts"
},
{
    "text": "Which of the following is NOT a real dinosaur?",
    "a": "Pterodactyl",
    "b": "Chicken",
    "c": "Velociraptor",
    "d": "Dryosaurus",
    "answer": "Pterodactyl"
},
{
    "text": "Where is the ancient city of Troy located in modern times?",
    "a": "Greece",
    "b": "Egypt",
    "c": "Turkey",
    "d": "Italy",
    "answer": "Turkey"
},
{
    "text": "Kunta Kinte is a central character in which famous novel and television miniseries?",
    "a": "12 Years a Slave",
    "b": "Roots: The Saga of an American Family",
    "c": "The Underground Railroad",
    "d": "Beloved",
    "answer": "Roots: The Saga of an American Family"
},
{
    "text": "Which of these countries was NOT a republic of the Socialist Federal Republic of Yugoslavia?",
    "a": "Kosovo",
    "b": "Slovenia",
    "c": "Albania",
    "d": "Montenegro",
    "answer": "Albania"
},
{
    "text": "What does the acronym FOMO stand for?",
    "a": "Fear Of Moving On",
    "b": "Fear Of Missing Out",
    "c": "Feeling Of Missing Opportunities",
    "d": "Forget Only Move On",
    "answer": "Fear Of Missing Out"
},
{
    "text": "How long is the Ring Road (Route 1) in Iceland?",
    "a": "Approximately 1,322 km (821 mi)",
    "b": "Approximately 1,953 km (1,213 mi)",
    "c": "Approximately 2,591 km (1,610 mi)",
    "d": "Approximately 3,214 km (1,997 mi)",
    "answer": "Approximately 1,322 km (821 mi)"
},
{
    "text": "What was the name of the first commercially sold video game in history, consisting of a ball bouncing off a platform?",
    "a": "Space Invaders",
    "b": "Tennis for Two",
    "c": "Breakout",
    "d": "Pong",
    "answer": "Pong"
},
{
    "text": "What is the largest number you can find in the video game Minesweeper?",
    "a": "6",
    "b": "7",
    "c": "8",
    "d": "9",
    "answer": "8"
},
{
    "text": "What was depicted on the world's first postage stamp, the Penny Black (1840)?",
    "a": "Queen Victoria",
    "b": "the union flag of the United Kingdom",
    "c": "An eagle",
    "d": "A black penny coin",
    "answer": "Queen Victoria"
},
{
    "text": "What does the Lakota word \"Tatanka\" mean?",
    "a": "Thank you",
    "b": "Enemy",
    "c": "American bison",
    "d": "Sacred mountain",
    "answer": "American bison"
},
{
    "text": "What is the HTTP status code for \"Unauthorized\"?",
    "a": "400",
    "b": "401",
    "c": "403",
    "d": "404",
    "answer": "401"
},
{
    "text": "As of June 2026, how many people have walked on the Moon?",
    "a": "6",
    "b": "12",
    "c": "24",
    "d": "28",
    "answer": "12"
},
{
    "text": "What does VPN stand for?",
    "a": "Virtual Private Network",
    "b": "Visual Processing Node",
    "c": "Verified Public Network",
    "d": "Virtual Protocol Navigation",
    "answer": "Virtual Private Network"
},
{
    "text": "What did Louis Braille invent in the first half of the 19th century?",
    "a": "A device for sending messages over long distances",
    "b": "A way to record and play back sound",
    "c": "A machine for typing documents",
    "d": "An alphabet made of tiny bumps",
    "answer": "An alphabet made of tiny bumps"
},
{
    "text": "What was the first video to reach one billion views on YouTube?",
    "a": "\"Baby\" by Justin Bieber",
    "b": "\"Gangnam Style\" by PSY",
    "c": "\"Despacito\" by Luis Fonsi",
    "d": "\"Hello\" by Adele",
    "answer": "\"Gangnam Style\" by PSY"
},
{
    "text": "According to historical rankings by peak land area, which empire is considered the largest in human history?",
    "a": "The Mongol Empire",
    "b": "The Russian Empire",
    "c": "The Roman Empire",
    "d": "The British Empire",
    "answer": "The British Empire"
},
{
    "text": "Which country was the last state to be admitted as a member of the United Nations on July 14, 2011?",
    "a": "Montenegro",
    "b": "South Sudan",
    "c": "Kosovo",
    "d": "Eritrea",
    "answer": "South Sudan"
},
{
    "text": "What was the first metal used by humans?",
    "a": "Iron",
    "b": "Bronze",
    "c": "Gold",
    "d": "Copper",
    "answer": "Copper"
},
{
    "text": "Who was crowned emperor on Christmas night in the year 800 AD?",
    "a": "Otto I",
    "b": "Pepin the Short",
    "c": "Charlemagne (Charles the Great)",
    "d": "Louis the Pious",
    "answer": "Charlemagne (Charles the Great)"
},
{
    "text": "Who approved the Magna Carta?",
    "a": "King Richard the Lionheart",
    "b": "King John of England",
    "c": "King Henry VIII",
    "d": "King William the Conqueror",
    "answer": "King John of England"
},
{
    "text": "Who wrote the Aeneid?",
    "a": "Homer",
    "b": "Sophocles",
    "c": "Ovid",
    "d": "Virgil",
    "answer": "Virgil"
},
{
    "text": "Who holds the record for the longest reign of any monarch of a sovereign state, lasting 72 years and 110 days?",
    "a": "Queen Elizabeth II (United Kingdom)",
    "b": "Sobhuza II (Eswatini)",
    "c": "Louis XIV (France)",
    "d": "Johann II (Liechtenstein)",
    "answer": "Louis XIV (France)"
},
{
    "text": "What is the name of the giant spider in The Lord of the Rings?",
    "a": "Shelob",
    "b": "Ungoliant",
    "c": "Aragog",
    "d": "Attercop",
    "answer": "Shelob"
},
{
    "text": "How many mountains are officially recognized as 'eight-thousanders' (mountains exceeding 8,000 meters in height)?",
    "a": "5",
    "b": "8",
    "c": "11",
    "d": "14",
    "answer": "14"
},
{
    "text": "On October 16, 1986, who became the first person to successfully climb all 14 eight-thousanders, the world's highest peaks over 8,000 meters?",
    "a": "Jerzy Kukuczka (Poland)",
    "b": "Edmund Viesturs (USA)",
    "c": "Reinhold Messner (Italy)",
    "d": "Mingma Sherpa (Nepal)",
    "answer": "Reinhold Messner (Italy)"
},
{
    "text": "Which of plays is part of the 20th century theatrical movement known as the Theatre of the Absurd?",
    "a": "Death of a Salesman",
    "b": "The Glass Menagerie",
    "c": "The Seagull",
    "d": "Waiting for Godot",
    "answer": "Waiting for Godot"
},
{
    "text": "What is the name of Rocky Balboa's girlfriend, and later wife, in the Rocky film series?",
    "a": "Adrian",
    "b": "Talia",
    "c": "Emma",
    "d": "Pauline",
    "answer": "Adrian"
},
{
    "text": "Which musical genre is generally written in 3/4 time and derives its name from that triple meter?",
    "a": "Polka",
    "b": "March",
    "c": "Waltz",
    "d": "Tango",
    "answer": "Waltz"
},
{
    "text": "What is the name of the precious fabric obtained from the soft undercoat of goats raised in the border region between India, Pakistan, and China?",
    "a": "Shahtoosh",
    "b": "Mohair",
    "c": "Cashmere",
    "d": "Angora",
    "answer": "Cashmere"
},
{
    "text": "In 1963, American commercial artist Harvey Ross Ball invented a design for a pin-back button. What is that iconic design?",
    "a": "The peace sign",
    "b": "The smiley face",
    "c": "The heart symbol",
    "d": "The recycling symbol",
    "answer": "The smiley face"
},
{
    "text": "Who was the first king of Rome?",
    "a": "Romulus",
    "b": "Remus",
    "c": "Julius Caesar",
    "d": "Augustus",
    "answer": "Romulus"
},
{
    "text": "Which of these types of waves propagates the farthest and best over long distances?",
    "a": "Radio AM",
    "b": "Radio FM",
    "c": "5G",
    "d": "Wi-Fi 5 GHz",
    "answer": "Radio AM"
},
{
    "text": "Which of these alphabets has the most letters?",
    "a": "Cyrillic (Russian)",
    "b": "Greek",
    "c": "English (Latin)",
    "d": "Armenian",
    "answer": "Armenian"
},
{
    "text": "Which of these things would you NOT see flying in a Disney cartoon?",
    "a": "An elephant",
    "b": "A house",
    "c": "A carpet",
    "d": "A car",
    "answer": "A car"
},
{
    "text": "Which biblical figure is recorded as the oldest person, living 969 years, and whose name has become synonymous with great age?",
    "a": "Noah",
    "b": "Abraham",
    "c": "Methuselah",
    "d": "Jared",
    "answer": "Methuselah"
},
{
    "text": "Who is the main author of the song \"Miss You\" (in co-production with Robin Schulz)?",
    "a": "Oliver Tree",
    "b": "David Pramik",
    "c": "Steve Aoki",
    "d": "Marshmello",
    "answer": "Oliver Tree"
},
{
    "text": "A large nose is a defining feature of which literary character?",
    "a": "Quasimodo",
    "b": "Cyrano de Bergerac",
    "c": "D'Artagnan",
    "d": "Edmond Dantès",
    "answer": "Cyrano de Bergerac"
},
{
    "text": "Which of these is not the name of a US state?",
    "a": "Missouri",
    "b": "Minnesota",
    "c": "Michigan",
    "d": "Milwaukee",
    "answer": "Milwaukee"
},
{
    "text": "Transnistria is a breakaway state that is internationally recognized as part of which country?",
    "a": "Romania",
    "b": "Ukraine",
    "c": "Moldova",
    "d": "Russia",
    "answer": "Moldova"
},
{
    "text": "Which key is held down on a standard keyboard in order to capitalize characters?",
    "a": "Enter",
    "b": "Escape",
    "c": "Alt",
    "d": "Shift",
    "answer": "Shift"
},
{
    "text": "Which of these classic cocktails is NOT traditionally prepared with vodka?",
    "a": "Negroni",
    "b": "Bloody Mary",
    "c": "Black Russian",
    "d": "Cosmopolitan",
    "answer": "Negroni"
},
{
    "text": "Who do members of the Swiss Guard protect?",
    "a": "The Pope",
    "b": "The CEO of Rolex",
    "c": "The queen of England",
    "d": "The Geneva Basilica",
    "answer": "The Pope"
},
{
    "text": "In the classic board game BattleShip, what is the total number of 'hits' needed to sink all of your opponent's ships?",
    "a": "10",
    "b": "17",
    "c": "21",
    "d": "29",
    "answer": "17"
},
{
    "text": "According to Guinness World Records, which type of fruit or vegetable currently holds the title for the world's heaviest?",
    "a": "Jackfruit",
    "b": "Watermelon",
    "c": "Pumpkin",
    "d": "Eggplant",
    "answer": "Pumpkin"
},
{
    "text": "Which of these dog breeds is best known for its incredible speed and is specifically bred for sprinting and racing?",
    "a": "Siberian Husky",
    "b": "Greyhound",
    "c": "Bulldog",
    "d": "Basset Hound",
    "answer": "Greyhound"
},
{
    "text": "What gas are modern Zeppelin airships typically filled with to achieve lift?",
    "a": "Hydrogen",
    "b": "Helium",
    "c": "Oxygen",
    "d": "Nitrogen",
    "answer": "Helium"
},
{
    "text": "What did Lady Godiva do to protest against the taxes imposed by her husband on the people of Coventry?",
    "a": "She refused to pay the taxes and went to live in a monastery",
    "b": "She challenged her husband to a duel",
    "c": "She rode naked through the streets of the town",
    "d": "She led a revolt of the townspeople",
    "answer": "She rode naked through the streets of the town"
},
{
    "text": "What is the speed of light in a vacuum?",
    "a": "Approximately 3,000 km/s",
    "b": "Approximately 30,000 km/s",
    "c": "Approximately 300,000 km/s",
    "d": "Approximately 3,000,000 km/s",
    "answer": "Approximately 300,000 km/s"
},
{
    "text": "What was the birth name of Pope John Paul II?",
    "a": "Karol Józef Wojtyła",
    "b": "Jorge Mario Bergoglio",
    "c": "Joseph Ratzinger",
    "d": "Robert Francis Prevost",
    "answer": "Karol Józef Wojtyła"
},
{
    "text": "If May 29 is a Friday, what day of the week will June 2 be?",
    "a": "Thursday",
    "b": "Monday",
    "c": "Tuesday",
    "d": "Sunday",
    "answer": "Tuesday"
},
{
    "text": "Sugars belong to which category of macronutrients?",
    "a": "Carbohydrates",
    "b": "Proteins",
    "c": "Lipids (fats)",
    "d": "Nucleic acids",
    "answer": "Carbohydrates"
},
{
    "text": "What percentage of the global population is left-handed?",
    "a": "About 5%",
    "b": "About 11%",
    "c": "About 15%",
    "d": "About 25%",
    "answer": "About 11%"
},
{
    "text": "How many sides does an isosceles triangle have?",
    "a": "1",
    "b": "2",
    "c": "3",
    "d": "4",
    "answer": "3"
},
{
    "text": "Which of these countries is NOT bordered by the sea (is landlocked)?",
    "a": "Ethiopia",
    "b": "Pakistan",
    "c": "Ecuador",
    "d": "Bulgaria",
    "answer": "Ethiopia"
},
{
    "text": "What is the name of the cocktail that James Bond invents in the 1953 novel Casino Royale, made with gin, vodka, and Kina Lillet (now substituted with Lillet Blanc)?",
    "a": "Martini (shaken, not stirred)",
    "b": "Vesper",
    "c": "Black Russian",
    "d": "Cosmopolitan",
    "answer": "Vesper"
},
{
    "text": "Which car is most famously associated with James Bond, first appearing in the 1964 film Goldfinger?",
    "a": "Aston Martin DB5",
    "b": "Lotus Esprit S1",
    "c": "Bentley S2 Continental",
    "d": "Rolls-Royce Phantom III",
    "answer": "Aston Martin DB5"
},
{
    "text": "Which of the following vitamins does NOT actually exist as a scientifically recognized vitamin?",
    "a": "Vitamin D",
    "b": "Vitamin E",
    "c": "Vitamin F",
    "d": "Vitamin K",
    "answer": "Vitamin F"
},
{
    "text": "What is the time difference between India (IST) and Nepal (NPT) in standard time?",
    "a": "Nepal is 15 minutes ahead of India",
    "b": "Nepal is 30 minutes ahead of India",
    "c": "Nepal is 60 minutes ahead of India",
    "d": "They have the same time zone (no difference)",
    "answer": "Nepal is 15 minutes ahead of India"
},
{
    "text": "The super-prison of Alcatraz is located on an island off the coast of which city?",
    "a": "Los Angeles",
    "b": "San Francisco",
    "c": "San Diego",
    "d": "Seattle",
    "answer": "San Francisco"
},
{
    "text": "What is a paternoster?",
    "a": "A traditional Catholic prayer bead",
    "b": "A type of continuous-loop elevator without doors that never stops",
    "c": "A medieval architectural support pillar",
    "d": "A 19th-century Italian musical instrument",
    "answer": "A type of continuous-loop elevator without doors that never stops"
},
{
    "text": "Kuduro is an electronic music genre from the late 1980s, characterized by fast and infectious rhythms. Which country did it originate from?",
    "a": "Brazil",
    "b": "Portugal",
    "c": "Angola",
    "d": "Jamaica",
    "answer": "Angola"
},
{
    "text": "Which of these bones is NOT present in the canine skeleton?",
    "a": "Clavicle",
    "b": "Humerus",
    "c": "Ulna",
    "d": "Tarsus",
    "answer": "Clavicle"
}
]

existing = Question.objects.count()
if existing == len(questions):
    print(f"{existing} questions already present in the database; skipping insert.")
else:
    print(f"{existing} questions currently in the database; have {len(questions)}  new questions...")
    Question.objects.all().delete()
    for q in questions:
        Question.objects.create(**q)

    print(len(questions), "questions inserted successfully!")
