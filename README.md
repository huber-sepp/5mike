# 5mike

Arma Reforger Missionsbau in 5 Minuten

## Reinkopieren, zurecht rücken, deployen

- Von Steam die Reforger Tools installieren.

- Alle Dependencies müssen in Reforger herunterladen und in der Workbench als Projekt der Liste hinzufügen.
https://reforger.armaplatform.com/workshop/695C85A248D888BC-5mikeDependencies

- Zusätzlich evtl. eine gemoddete Karte. 
https://reforger.armaplatform.com/workshop/6626CB89B6B9C913-Bystrany

- Wenn man Dependencies oder etwas an Ordnern ändert, muss man die Workbench neustarten.

- Neues Projekt anlegen mit 5mike_dependencies.

![alt text](https://github.com/huber-sepp/5mike/blob/main/Bilder/poject-dependencies.png)

Nach dem ersten Speichern liegt das Ding dann Standardmäßig hier 
C:\Users\ DEIN BENUTZERNAME \Documents\My Games\ArmaReforgerWorkbench\addons

Links in der Seitenleiste eine "world" auswählen und die .ent Datei in dem Ordner doppelklicken. 

Im  World-Editor auf File->New World->Sub-scene.
Dann "Save As" im Projekt-Ordner unter "worlds". 
Dort liegt dann eine .ent, eine .ent.meta und ein Ordner mit _Layers. Die .layer-Dateien darin enthalten am Ende alles was im World Editor platziert wird.

Die neu abgespeicherte default.layer mit denen aus \ArmaReforgerWorkbench\addons\5mike_kolguyev\worlds\5mike_Layers ersetzen.
Workbench neustarten.

WICHTIG: Links im five_mike-Layer das Modul AIWorld auswählen. Im Resource Browser im Ordner der jeweiligen Map nach nmn suchen und die gefundenen Dateien in den NavmeshWorldComponents der AIWorld ersetzen.

Die Dinge im five_mike-Layer an den gewünschten Startpunkt ziehen. 

Die "Areas" im scenario-Layer an die gewünschten Missionsziele ziehen. 

Links in der Hirarchie "SlotAI" ist für Gruppen, "Slot" für seelenlose Dinge.

Bei allen davon kann man rechts im Scenario-Modul das "Object To Spawn" definieren.

In den eigenen Projekt-Ordner kopieren: \ArmaReforgerWorkbench\addons\5mike_kolguyev\Missions.

In der Workbench passend umbenennen und Doppelklick auf die conf. Darin die eigene World auswählen.

Speichern und im Workshop deployen.


## Tipps

- Prefabs: Alles was wir im Resource Browser suchen, nehmen wir aus dem Ordner "Prefabs". Das sind funktionsfähig vorkonfigurierte Pakete, wo alle nötigen Komponenten schon dabei sind. Rechts in der Leiste kann man diese samt Unterkomponenten konfigurieren. Dabei nichts löschen, sondern nur deaktivieren.
- Prefabs und fast alles andere kann man im eigenen Mod "overriden". So kann man z.B. das Loadout von vorhandenen Einheiten ändern
- Bebilderung: In der Workbench links unten gibt es die Buttons "Import" und "Create". Hier kann man JPGs importieren und in .edds Dateien umwandeln lassen. (https://forums.bohemia.net/forums/topic/272105-how-to-create-an-edds-file-for-images/)

