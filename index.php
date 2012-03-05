<?php

include_once dirname(__FILE__).'/../utils/utils.inc';
include_once dirname(__FILE__).'/../database/fact.inc';

define('FFF_GET_FACT', 'getFact');
define('FFF_GET_GUID', 'getGuid');

function parseOptions() {
  $options = array();
  try {
    $options['callback'] = Utils::getCallback();
    $options['method'] = Utils::getMethod();
  } catch (Exception $e) {
    echo $e->getMessage();
    exit($e->getCode());
  }

  // If getFact id should be present as well.
  if ($options['method'] == FFF_GET_FACT) {
   try {
      $options['id'] = Utils::getGUID();
    } catch (Exception $e) {
      echo $e->getMessage();
      exit($e->getCode());
    }
  }

  return $options;
}

function returnJson($result) {
   header('Content-type: application/json');
   echo json_encode($result);
}

$options = parseOptions();
switch ($options['method']) {
  case FFF_GET_FACT:
    $fact = new Fact();
    returnJson($fact->load($options['id']));
    break;

  case FFF_GET_GUID:
    returnJson(array('id' => Fact::getGUID()));
    break;

  default:
    echo 'Method is not available...';
    exit(-1);
    break;
}

