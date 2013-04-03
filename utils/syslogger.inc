<?php
/**
 * @file
 * This logger is based on the syslog module found in Drupal 7 core and modified
 * for the use case here.
 */

if (defined('LOG_LOCAL0')) {
  define('DEFAULT_SYSLOG_FACILITY', LOG_LOCAL0);
}
else {
  define('DEFAULT_SYSLOG_FACILITY', LOG_USER);
}

/**
 * Implements SysLogger class as a singleton class.
 */
class SysLogger {
  private $init = FALSE;

  private function __construct() {
    if ($this->init) {
      $this->init = TRUE;
      $default_facility = defined('LOG_LOCAL0') ? LOG_LOCAL0 : LOG_USER;
      openlog('fff_service', LOG_NDELAY, $default_facility);
    }
  }

  /**
   * Call this method to get the logger class.
   */
  public static function getLogger(){
    static $instance = null;
    if ($instance === null) {
      $instance = new SysLogger();
    }
    return $instance;
  }

  public function log($api_key, $message, $severity) {
    $message = strtr('!api_key|!timestamp|!ip|!request_uri|!query|!message', array(
      '!api_key'     => $api_key,
      '!timestamp'   => date('r', $_SERVER['REQUEST_TIME']),
      '!ip'          => $_SERVER['REMOTE_ADDR'],
      '!request_uri' => $_SERVER['REQUEST_URI'],
      '!query'       => $_SERVER['QUERY_STRING'],
      '!message'     => strip_tags($message),
    ));

    syslog($severity, $message);
  }
}
