<?php
require(__DIR__ . "/../vendor/autoload.php");
use phpFastCache\CacheManager;
use GuzzleHttp\Client as HttpClient;

// Setup File Path on your config files
CacheManager::Files([
    "path" => __DIR__ . '/../tmp',
]);

// In your class, function, you can call the Cache
$InstanceCache = CacheManager::getInstance('files');

$response = [];

// numerically indexed array of places

if (isset($_GET['lat']) && isset($_GET['lon'])) {
    $key = $_GET['lat'] . $_GET['lon'];
    $cachedString = $InstanceCache->getItem($key);
    if (is_null($cachedString->get())) {
        $client = new HttpClient();
        $res = $client->request('GET', "http://api.openweathermap.org/data/2.5/weather", [
            'query' => [
                'lat' => $_GET['lat'],
                'lon' => $_GET['lon'],
                'APPID' => "f78b83f9bcf8582ece948156d533f92e",
            ],
        ]);
        $response['code'] = $res->getStatusCode();
        $response['reason'] = $res->getReasonPhrase();
        $response['source'] = 'origin';
        if ($res->getStatusCode() == 200) {
            $content = $res->getBody()->getContents();
            $cachedString->set($content)->expiresAfter(600);//in seconds, also accepts Datetime
            $InstanceCache->save($cachedString); // Save the cache item just like you do with doctrine and entities
            $response['body'] = json_decode($content);
        } else {
            $response['body'] = 'not 200';
        }
    } else {
        $response['source'] = 'cache';
        $response['body'] = json_decode($cachedString->get());
    }
}

// output places as JSON (pretty-printed for debugging convenience)
header("Content-type: application/json");
print(json_encode($response, JSON_PRETTY_PRINT));
