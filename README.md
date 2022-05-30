# EAI2_Network_Programming

## introductie
  
  <p> Dit project heeft als doel meerdere alleenstaande services te hebben die samen tot een network kunnnen worden samengegoten.
  Het project draait vooral rond zmq sockets om de communicatie op te stellen.</p>
  
  <p> voor hulp bij gebruik, typ 'help' in de client console </p>
  
  ### Compilen
  
  <p> Het project moet met cmake gecompiled worden en heeft enkele extra libraries: de zmq library en de Qt6::core library. 

## Service 1

<p> Deze service kan de RGB waarden uit een .bmp halen en deze in 3 images steken van 1 kleur. <p>

### benodigdheden Server
  
  <p> Bij deze service is het meegeleverde programma.exe nodig, die moet in dezelfde map als de executable steken. <p>
    
### benodigdheden Client
    
  <p> om foto's te bewerken als client moeten de fotos in dezelfde map gestoken worden, de fotos moeten wel een resolutie hebben van een veelvoud van 4.
    Om dit bestand te versturen moet de naam met extentie ingevoerd worden</p>
  
## Service 2

<p> dit is een simpele ping-pong service, om te testen moet ping worden ingevoerd waarna pong terug wordt gegeven. </p>

