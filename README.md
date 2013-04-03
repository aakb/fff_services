# Finurlige Fakta Web-service
The project has been build around a web-service that makes it possible to make applications that utilises facts in different ways and this document explains the methods available at the service.

## Obtaining an API key
To access the web-service you first need to obtain an API key for the service. This key is personal and should not be shared with others. The API key is free and all you need is to register for it at http://finurligefakta.dk/api-key after which you will receive an e-mail with your key.

__Note__: If we detect that you are calling the web-service to aggressively or to may times in a row we may disable your key to ensure that the service will work for all users. You will receive a mail notification, if we disable your key.

## The web-service
The service is located at http://service.finurligefakta.dk/ and returns all responses as JSON due to the fact that it was developed as a backend for a JavaScript widget. If a callback parameter is given the services will handle the response as a [JSON-P](https://en.wikipedia.org/wiki/JSONP) to get around cross domain requests.

All request should as minimum have these query parameters when accessing the service and.
```javascript
http://service.finurligefakta.dk/?method=<METHOD>&callback=<FUNCTION>&api-key=<KEY>
```

The service offers these methods.

1. getGuid
2. getFact
3. getFacts
4. getCount

-------------------------

### 1. getGuid
Returns a random GUID matching a published fact object.

#### Call
```javascript
http://service.finurligefakta.dk/?method=getGuid&api_key=<KEY>callback=<CALLBACK>
```

#### Response (JSON)
If the callback is given (testCallback) the response will be encapsulated.
```javascript
testCallback({"guid":"737"})
```

If no callback is given the function callback is omitted.
```javascript
{"guid":"737"}
```

-------------------------

### 2. getfact 
Returns a fact object based on a GUID as a JSON-P response if a callback is given.

#### Call
```javascript
http://service.finurligefakta.dk/?method=getGuid&api_key=<KEY>callback=<CALLBACK>&guid=<GUID>
```

#### Response (JSON)
This is the result of the call with the GUID set to 737.
```javascript
{
  "guid" : "737",
  "title" : "Skidenlørdag",
  "author" : "Koue, Anette",
  "time" : "1341220978",
  "content" : "Det er almindeligt at spise skidne æg påskelørdag. Navnet kommer af, at man på denne dag skal gøre rent efter to helligdage og før de to næste.",
  "organization" : "Viborg Bibliotekerne",
  "inspirations" : [{
    "title" : "Skikke og symboler",
    "url" : "http://www.dafos.dk/formidling/laesehjoernet/paaske/skikke-og-symboler.aspx"
  }],
  "sources" : [{
    "title" : "Skidne æg ",
    "url" : "http://www.denstoredanske.dk/Mad_og_drikke/Gastronomi/Mejerivarer/skidne_æg"
  }],
  "keywords" : ["påskefester", "påsketraditioner", "skidenlørdag", ""]
}
```

-------------------------

### 3. getFacts
Returns an array of fact objects based on the offset and count parameters. The _offset_ set a starting point in the list of facts and _count_ set how many form the _offset_ you want. This way you can load x facts at the time. You can use the _getCount_ call to get the number of facts currently available 

#### Call
```javascript
http://service.finurligefakta.dk/?method=getFacts&api-key=<KEY>&callback=<CALLBACK>&offset=<INT>&count=<INT>
```

#### Response (JSON)
A call to the service with an offset set to 20 and a count of 2 will give an response as below where fact object is the same format as under _getFact_ above.
```javascript
{
  "557" : { FACT OBJECT }, 
  "558" : { FACT OBJECT }
}
```

-------------------------

### 4. getCount
Returns the number of facts currently in the database.

#### Call
```javascript
http://service.finurligefakta.dk/?method=getCount&api-key=<KEY>&callback=<CALLBACK>
```

#### Response (JSON)
```javascript
{
  "count" : "562"
}
```























