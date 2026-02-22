# Mini-UNIX-tools
Reimplementation of basic Unix utilities (cat, grep, zip, unzip) and a parallelized version of zip (parallelzip) in C.
This project demonstrates systems programming concepts including file I/O, string processing, binary data handling, and error handling.

---

## test.txt
`test.txt` is a simple sample file used for testing the functionality of all the mini tools.
It contains short lines, single-character lines, repeated characters (useful for RLE compression testing) and a sentence containing the word "get" to verify search functionality with grep.


# Descriptions in English

## cat
The `cat` program takes file paths as command-line arguments and prints their contents sequentially to the console.
The program notifies the user if too few arguments are provided, in which case the process terminates normally and returns exit code 0.
If an error occurs while opening a file, the process terminates, prints an error message to the user, and returns exit code 1.

## grep
The `grep` program takes as arguments first the search string and then the files in which the string is searched line by line.
The getline() function dynamically allocates memory for each line read from the file. Then the strstr() function compares the line to the given keyword, and the line is printed to the user if it contains the specified string.
The program notifies the user if too few arguments are provided or if a specified file cannot be opened. In these cases, the program terminates and returns exit code 1.
Grep also works if the user provides an empty string as the search keyword. In that case, the program prints all lines from all given files.
The program can also search for the keyword from standard input (stdin) if the user provides only the keyword as an argument and no file names. This feature also works if the keyword is an empty string, in which case all input lines are printed. To exit the program in this mode, the user must send an interrupt signal, for example by pressing Ctrl-C.

## zip
The `zip` program takes at least one command-line argument and compresses the file using run-length encoding (RLE). The result is printed to standard output (stdout).
First, zip opens the file and reads the first character using the fgetc() function. If the next character is the same as the previous one, the counter is incremented by 1.
When the next character differs from the previous one, the current count is written to stdout in binary format, followed by the character in normal ASCII format. The counting then restarts, and the next count and character are written.
This process continues until all provided files have been processed.

## unzip
The `unzip` program takes at least one command-line argument, decompresses the file, and prints the original text to stdout.
Unzip opens the file, reads a binary number, and stores it in a variable. Then it reads the corresponding ASCII character and prints that character to stdout as many times as indicated by the number.
This process continues until all files have been processed.

## parallelzip
The `parallelzip` program utilizes temporary text files, each of which stores the compressed version of a file. A separate temporary file is created for every file provided as a command-line argument. The program compresses all given files in parallel at the same time. This speeds up the compression so that the total time spent compressing is (approximately) the same as the time it would take to compress only the largest file in a normal sequential implementation.
In the main function, memory is allocated for a ThreadData struct for each filename provided as a command-line argument. A pointer to the opened file is stored in the struct, along with a temporary filename used for writing the compressed output. After this, a thread is created, and the same process is repeated for the next file.
Each thread runs the zip function, which compresses the file and writes the result to a file. After writing, the struct memory is freed and execution returns to the main function.
In the main function, the temporary file is printed to stdout as the threads return. After printing the contents of the temporary file, the temporary file is deleted, and the program waits for the next temporary file to finish. This is repeated until all files have been printed.

---

# Descriptions in Finnish

## cat
`cat` ohjelma ottaa komentoriviargumentteina tiedostopolkuja ja tulostaa niiden sisällön peräkkäin konsoliin.  
Ohjelma ilmoittaa käyttäjälle, mikäli argumentteja on liian vähän, jolloin prosessi lopettaa toimintansa ja palaa normaalisti arvolla 0. Jos kuitenkin tiedoston avaamisessa ilmenee virhe, lopettaa prosessi toiminnan, tulostaa käyttäjälle virheilmoituksen, ja palaa koodilla 1.

## grep
`grep` ohjelma ottaa argumentteina ensin etsittävän merkkijonon ja sitten tiedostoja, joista merkkijonoja etsitään rivi kerralla.  
Funktio getline() varaa dynaamisesti muistia tiedostosta löydetylle riville. Sitten funktio strstr() vertailee riviä annettuun avainsanaan, minkä jälkeen tulostetaan käyttäjälle rivi, jos siitä löytyi kyseinen merkkijono. 
Ohjelma ilmoittaa käyttäjälle, jos annettuja argumentteja on liian vähän tai annettua tiedostoa ei voida avata. Tällöin ohjelma loppuu, ja palaa arvolla 1. 
Grep toimii myös, jos käyttäjä syöttää avainsanaksi tyhjän merkkijonon. Silloin ohjelma tulostaa kaikkien annettujen tiedostojen kaikki rivit. 
Ohjelma pystyy myös etsimään stdin-syötteestä käyttäjän konsoliin kirjoittamista riveistä avainsanaa, jos käyttäjä antaa argumenttina vain avainsanan eikä tiedostonimeä. Tämä ominaisuus toimii myös, jos käyttäjä antaa tyhjän merkkijonon avainsanaksi, sillä silloin ohjelma tulostaa kaikki syötetyt rivit. Tällöin ohjelmasta poistuakseen, käyttäjän on suoritettava signaali keskeytys, esim. painamalla ctrl-C.

## zip
`zip` ohjelma ottaa vähintään yhden komentorivi argumentin, kompressoi tiedoston käyttäen run length encoding. Tämä tulos tulostetaan sitten stdout:iin. 
Ensin zip avaa tiedoston, sitten lukee ensimmäisen merkin fgetc() funktiolla. Jos seuraava merkki on samanlainen kuin edellinen, lisätään counteriin 1. Kun seuraava merkki on joku muu kuin edellinen, kirjoitetaan stdout:iin sen hetkinen count numero binäärimuodossa ja merkki tavallisessa ASCII muodossa. Sitten aloitetaan laskeminen alusta, ja kirjoitetaan seuraava numero ja merkki. Tämä jatkuu, kunnes annetut tiedostot loppuvat. 

## unzip
`unzip` ohjelma ottaa vähintään yhden komentorivi argumentin, dekompressoi tiedoston ja tulostaa alkuperäisen tekstin stdout:iin.
Unzip avaa tiedoston, lukee binäärimuotoisen numeron ja tallentaa sen muuttujaan. Sen jälkeen luetaan ASCII merkki, ja tulostetaan numeron osoittaman verran merkkejä stdout:iin.
Tämä jatkuu, kunnes tiedostot päätyvät.

## parallelzip
`parallelzip` ohjelmassa hyödynnetään tilapäisiä tekstitiedostoja, joihin jokaiseen kirjoitetaan tiedoston zipattu muoto. Kaikille komentoriviargumenteissa annetuille tiedostoille luodaan oma tilapäinen tiedosto. Ohjelma siis zippaa kaikkia annettuja tiedostoja rinnakkaisesti samaan aikaan. Tämä nopeuttaa zippaamista siten, että zippaamiseen kulut aika on (noin) sama, mikä kuluisi normaalissa peräkkäisessä zippaamisessa pelkästään pisimmän tiedoston zippaamiseen. 
Main-funktiossa varataan muistia ThreadData structille jokaista komentoriville annettua tiedostonimiargumenttia kohden. Structiin lisätään osoitin avattuun tiedostoon, ja zipatun tiedoston kirjoittamista varten tilapäinen tiedostonimi. Tämän jälkeen luodaan threadi, ja tehdään sama seuraavalle tiedostolle.  
Jokainen threadi ajaa zip-funktiota, joka zippaa tiedoston, ja kirjoittaa tuloksen tiedostoon. Kirjoituksen jälkeen vapautetaan structin muisti ja palataan main-funktioon. 
Main-funktiossa tulostetaan stdout:iin tilapäinen tiedosto threadien palatessa. Tilapäisen tiedoston sisällön tulostamisen jälkeen poistetaan tilapäinen tiedosto, ja siirrytään odottamaan seuraavan tilapäisen tiedoston valmistumista. Tätä toistetaan, kunnes kaikki tiedostot on tulostettu. 
