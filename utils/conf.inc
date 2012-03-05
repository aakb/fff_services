<?php

/*
 * TODO: Create the config object as a singleton pattern.
 */

class Conf {

  // Database configuration
  private $database = NULL;
  private $server   = NULL;
  private $user     = NULL;
  private $passwd   = NULL;

  // Load configuration
  public function __construct() {
    include_once dirname(__FILE__).'/settings.inc';    
    $this->database = $settings['database'];
    $this->server = $settings['server'];
    $this->user = $settings['user'];
    $this->passwd = $settings['passwd'];
  }

  public function getDB() {
    return $this->database;
  }

  public function getServer() {
    return $this->server;
  }

  public function getUser() {
    return $this->user;
  }

  public function getPassword() {
    return $this->passwd;
  }
  // End database configuration

  // Define the system locations
  private $webroot = null;
  public function getWebroot() {
    if ($this->webroot == null) {
      $path = substr(dirname(__FILE__), 0, strlen(dirname(__FILE__)) - 5);
      $this->webroot = substr($path, strlen($_SERVER['DOCUMENT_ROOT']));

      // Correction of webroot
      if ($this->webroot == '/') {
        $this->webroot = '';
      }
    }
    return $this->webroot;
  }

  private $admin_page = '/user/admin.php';
  public function getAdminPage() {
    return $this->getWebroot() . $this->admin_page;
  }
  // End define the system locations

  // Cookie config
  private $cookie_name   = 'finurlig_fakta';
  private $cookie_expire = 30; // Mins
  public function getCookieName() {
    return $this->cookie_name;
  }

  public function getCookieExpire() {
    return $this->cookie_expire * 60; // Secs
  }
  // End cookie config
}

$conf = new Conf();
