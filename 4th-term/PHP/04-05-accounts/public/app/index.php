<?php


require '../../vendor/autoload.php';

// GUZZLE

$client = new GuzzleHttp\Client([
    'base_uri' => 'http://localhost/~maciej/project/']);
$response = $client->get('index.php/api/account/all');
$body = $response->getBody();
$bodyObject = json_decode($body);

//$res = $client->request('GET', 'localhost:8888/api/account/all', [
//    ‘headers’ => [
//        ‘Accept’ => ‘application/json’,
//        ‘Content-type’ => ‘application/json’
//    ]]);

// TWIG

$loader = new \Twig\Loader\FilesystemLoader('../templates');
$twig = new \Twig\Environment($loader, [
    'cache' => '../compilation_cache',
]);
$template = $twig->load('users.html.twig');

echo $template->render( ['accounts' => $bodyObject]);