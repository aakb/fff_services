<?php

include_once dirname(__FILE__).'/utils/utils.inc';
include_once dirname(__FILE__).'/database/fact.inc';

define('FFF_ERROR_CALLBACK', 'error');
define('FFF_GET_FACT', 'getfact');
define('FFF_GET_GUID', 'getguid');

function parseOptions() {
  $options = array();
  try {
    $options['method'] = Utils::getMethod();
    $options['callback'] = Utils::getCallback();
  } catch (Exception $e) {
    returnJsonError(array(
      'msg' => $e->getMessage(),
      'code' => $e->getCode(),
    ));
  }

  // If getFact id should be present as well.
  if ($options['method'] == FFF_GET_FACT) {
   try {
      $options['guid'] = Utils::getGUID();
    } catch (Exception $e) {
      returnJsonError(array(
        'msg' => $e->getMessage(),
        'code' => $e->getCode(),
      ));
    }
  }

  return $options;
}

function returnJsonError($result) {
   header('Content-type: application/json');
   echo FFF_ERROR_CALLBACK . '(' . json_encode($result) . ')';
   exit(isset($result['code']) ? $result['code'] : -1);
}

function returnJson($result, $callback) {
   header('Content-type: application/json');
   echo $callback . '(' . json_encode($result) . ')';
}

$options = parseOptions();
switch ($options['method']) {
  case FFF_GET_FACT:
    try {
      $fact = new Fact(array('guid' => $options['guid']));
      returnJson($fact->toJson(), $options['callback']);
    } catch (Exception $e) {
      returnJsonError(array(
        'msg' => $e->getMessage(),
        'code' => $e->getCode(),
      ));
    }
    break;

  case FFF_GET_GUID:
    returnJson(array('guid' => Fact::getGUID()), $options['callback']);
    break;

  default:
    returnJsonError(array(
      'msg' => 'Method is not available',
      'code' => -1,
    ));
    exit(-1);
    break;
}

