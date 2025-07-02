Opis kodu programu:
1. Funkcja SolarSystem::SolarSystem() inicjalizuje okno aplikacji o rozdzielczości 1280x960, konfiguruje kamerę 3D, 
ładuje tekstury dla Słońca, Księżyca i planet, generuje modele sferyczne dla tych obiektów oraz ustala parametry ruchu i rotacji planet w układzie słonecznym.
2. Funkcja SolarSystem::UpdateCamera() aktualizuje pozycję i orientację kamery 3D, umożliwiając obrót za pomocą prawego przycisku myszy oraz ruch w kierunkach góra/dół/lewo/prawo za pomocą klawiszy strzałek,
a także przybliżanie i oddalanie kamery klawiszami +/-, z zachowaniem płynnej nawigacji wokół centralnego punktu sceny.
3. Funkcja SolarSystem::run() zarządza główną pętlą programu, obsługując przełączanie trybu pełnoekranowego (F11), aktualizując pozycje i rotacje planet oraz Księżyca, renderując tło, modele 3D Słońca,
planet i Księżyca w trybie 3D, oraz wyświetlając tekstowe menu z instrukcjami sterowania.
4. Funkcja SolarSystem::DrawOrbitRing rysuje pierścień orbity o zadanym promieniu w przestrzeni 3D,
dzieląc okrąg na 100 segmentów i łącząc kolejne punkty za pomocą linii, korzystając z funkcji trygonometrycznych do obliczania pozycji w płaszczyźnie XZ wokół centralnego punktu.
