Zadanie jest podzielone na dwie zasadnicze części:

Implementacja kolejki priorytetowej na potrzeby komunikacji międzyprocesowej.
Na podstawie kolejek z punktu 1 stworzyć symulację urzędu.
 Kolejka priorytetowa
Kolejka będzie służyć do przesyłania wiadomości/komunikatów między procesami.

Mechanizm kolejek powinien zostać oparty o semafory i pamięć współdzieloną systemu Linux. W szczególności proszę zapoznać się z dokumentacją (manpages) następujących funkcji w języku C pod Linuxem: semget, semop, semctl, shmget, shmctl, shmat, shmdt.

Uwaga - zasoby systemowe (semafory, pamięć współdzielona) używane przez kolejkę powinny być bezwzględnie zwalniane. 'Wyciek' takich zasobów będzie surowo karany.

Mechanizm kolejek powinien zostać dostarczony jako biblioteka w języku C, tj. implementacja w oddzielnym pliku .c, oraz odp. plik nagłówkowy z sensownym interfejsem użytkownika (programisty) i dobrymi komentarzami dokumentującymi.

Kolejka powinna implementować następującą funkcjonalność:

Ograniczenie pojemności kolejki.
Mechanizm producent-konsument.
Działanie wg. zasady FIFO (First In First Out). Wyjątek stanowią komunikaty priorytetowe, które wyprzedzają wszystkie pozostałe.
Możliwość nieblokującego sprawdzenia czy kolejka jest pusta.
 Symulacja urzędu
Należy zaprojektować, zaimplementować i zwizualizować symulację urzędu w oparciu o poniższy szkic.

Urząd

Do projektu należy dołączyć makefile, który pozwoli na skompilowanie symulacji do pliku wykonywalnego.

 Opis symulacji
Każda 'kolejka do urzędnika' (odcinki zastrzałkowane) jest modelowana przez kolejkę o ograniczonej pojemności. Każda z nich powinna mieścić jednocześnie 10 petentów. Wykorzystanie kolejki powinno się odbywać przez interfejs wystawiany w bibliotece z poprzedniego punktu.

Kolejki pomiędzy urzędnikami 0 - 1 nie rozróżniają typu petentów i działają na podstawie klasycznej zasady FIFO (First In First Out).
Kolejki pomiędzy urzędnikami 1 - 2 powinny traktować petentów-biznesmenów priorytetowo - przenosić ich nad osoby prywatne.
Każdy urzędnik symulowany jest przez proces (na szkicu kropki). U każdego urzędnika petent jest obsługiwany przez ok. 1 sekundę czasu symulacji. Urzędnik może obsługiwać tylko jednego petenta na raz.

Urzędnik 0 odpowiedzialny jest za generację petentów i umieszczanie ich w kolejkach wyjściowych. Generowani petenci dzielą się na:
osoby prywatne
biznesmenów
Urzędnicy 1 są pierwszym 'szczebelkiem' machiny urzędniczej, każdy z nich pobiera petenta z kolejki wchodzącej, obsługuje go, a następnie umieszcza w kolejce wychodzącej.
Urzędnik 2 wybiera petenta z kolejki wejściowej, która zawiera więcej biznesmenów (w przypadku identycznej liczby wybór kolejki jest dowolny), obsługuje go, a następnie petent opuszcza urząd.
 Wizualizacja
Sposób wizualizacji symulacji jest kwestią otwartą - musi być ona jednak czytelna i pokazywać zmianę symulacji w czasie. Należy zwrócić uwagę na problem synchronizacji przy wizualizacji - niezależnie od wybranej metody wizualizacji powinna być ona płynna, w szczególności niedopuszczalna jest walka różnych procesów o dostęp do konsoli tekstowej. Nie oznacza to jednak, że istnieje wymóg wprowadzenia synchronizacji na potrzeby wizualizacji, problem można rozwiązać m.in. poprzez zastosowanie odpowiedniej metody wyświetlania, np. osobne konsole, animacje 2D, 3D, VR itp.
