# IVT Auto Temp Controller

**Sorry, Swedish only for now ;)**

## Disclaimer:
**Allt detta görs givetvis på egen risk! Inga garantier ges att funktionen kommer att vara densamma på alla typer av värmepannor.**

## Beskrivning:
Detta projekt implementerar automatisk extern styrning av yttertemperatur för t.ex. IVT490 och liknande värmepannor, bör fungera för samtliga system som använder yttertempgivare av typen NTC 4,7Kohm vid 25 grader. Systemet baseras på tre grundkomponenter: ESP8266-baserad controller, en MCP41100 digital resistor (0-100Kohm), Relä för in-/urkoppling av extern styrning. Resultatet blir en värmepanna som helt kan styras mot innertemperatur, justera värme upp/ner baserat på el-priset mm. 
Inkopplingen är gjord så att när Relä 1 är draget så aktiveras styrning med hjälp av den digitala resistorn, blir systemet spänningslöst eller slutar fungera av nån annan orsak så bör reläet släppa vilket gör att pannan då automatiskt faller tillbaka på den vanliga styrningen med hjälp av den egna temp-sensorn.

**OBS! Systemet är inte fristående utan kräver en del förutsättningar för att fungera, dessa listas här nedan.**

## Förutsättningar:
### 1.	Hårdvara:
-	Wemos D1 Mini eller liknande ESP8266-baserad controller
-	[MCP41100 Digital Resistor](https://www.electrokit.com/produkt/mcp41100-i-p-dip-8-digital-pot-100kohm-256-steg-spi/)
- Reläkort, [Dubbelt](https://www.electrokit.com/produkt/relakort-x2-5v-opto-isolerat/) eller [Enkelt](https://www.electrokit.com/produkt/relamodul-5v/)
-	Optional: kopplingsdäck av något slag för att koppla ihop kretsarna, [t.ex. detta](https://www.electrokit.com/produkt/kopplingsdack-270-anslutningar/)
![image](https://user-images.githubusercontent.com/23386303/210139057-4c5bb92e-2825-4982-aaab-553c5b3c5b73.png) ![image](https://user-images.githubusercontent.com/23386303/210139080-2b546aa6-e1b6-4e57-a20f-da3822a43a55.png)



### 2.	Styrsystem:
-	Fullt fungerande Home Assistant-system med följande tillägg: NodeRed, ESPHome
-	Relativt god kunskap i ovan listade system + tillägg, då denna instruktion endast kommer visa grunderna, detalj-justering måste göras från system till system.
-	Temperaturgivare för inomhus- samt utomhustemp, kopplad till Home Assistant

### 3.	Frivilligt, men rekommenderat:
-	Värmepanna uppkopplad mot Home Assistant.
- [Powersaver](https://powersaver.no/) i NodeRed som kan styra mellan olika "Modes" baserat på aktuellt el-pris, alternativt får man skapa en switch av nåt slag i Home Assistans som används för att manuellt väljer mellan olika "Modes".

## Installation:
-	Koppla ihop hårdvaran enl det elektriska schemat. Verifiera gärna funktionen först (se punkt 2 nedan) innan värmepannan kopplas in skarpt.
- Koppla din Wemos D1 mini till ESPHome och flasha med koden i ESPHome-mappen. Lägg till din nya enhet som en Integration i HomeAssistant, verifiera sedan med multimeter att motståndet över den digitala resistorn ändras när värdet på ”DigiPot” ändras. Testa även att reläet slår om ” IVT Temp Tweaker 1” aktiveras. Om dessa båda test fungerar kan pannan anslutas enl schemat.
- Skapa ett antal ”Helpers” i Home Assistant:
  - input_boolean.ngenic_style (För att aktivera eller deaktivera styrning via extern control)
  -	input_number.bestam_framledningstemp (för uträknad framl.temp)
  - input_number.bestam_utetemp (för uträknad utetemp)
  - input_number.onskad_temp_inne (För önskad innetemp)
  - input_number.givarpaverkan (För påverkan innetemp)
  - input_number.temp_justering_save (påverkan Save-mode)
  - input_number.temp_justering_boost (påverkan Boost-mode)
-	Starta NodeRed och importera de båda flödena som finns här under filer. 
-	Det ena (NodeRed Resistor Flow.json) hanterar omvandling från önskad utetemp till nivå på den digitala resistorn. Function Noden ”Temp->Framl” behöver kalibreras beroende på vald värmekurva i pannan, för kurva 6 i en IVT490 så gäller följande formel: ”y = -0.95x + 39 där y är framledningstemp och x är ute-temp”
-	Det andra (NodeRed Control Flow.json) hanterar styrning av önskad temp. Subflow noden ”Ngenic Style Sensors” behöver justeras så att sensorerna matchar aktuell setup. Här behöver man nog en del kunskap för att få ihop en fullt fungerande styrning som matchar just dina förutsättningar. Detta flöde skapar även ett antal sensorer som läggs till i Home Assistant för att visa status, setpoint-temp m.m
-	Lägg till ett Entity card i Home Assistant och lägg till dom entities som ska visas.


## Bilder
### Node Red Flow 1
![image](https://user-images.githubusercontent.com/23386303/210139199-7f188a97-bf67-4154-b49c-e9860ee29eb3.png)

### NodeRed flow 2
![image](https://user-images.githubusercontent.com/23386303/210139147-31ec0578-f581-43c6-932f-7ee6033d3696.png)

### Node Red Subflow
![image](https://user-images.githubusercontent.com/23386303/210139186-86e3ddf0-f48c-4890-997a-265b0af224e1.png)

### Home Assistant Entity Card
![image](https://user-images.githubusercontent.com/23386303/210139264-d7704026-cb69-499a-9b0b-596c15a3d04f.png)

