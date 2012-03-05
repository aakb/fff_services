<?php

include_once dirname(__FILE__).'/../utils/conf.inc';

/****************************************************
 * This class is implemented as a singleton pattern *
 ****************************************************/
class PDOMysql {

  protected $db = null;
  protected $server = null;
  protected $user = null;
  protected $passwd = null;
  protected $dbh;
  private static $instance;

  private function __construct() {
    // Load database configuration
    global $conf;
    $this->db = $conf->getDB();
    $this->server = $conf->getServer();
    $this->user = $conf->getUser();
    $this->passwd = $conf->getPassword();

    // Connect to the database
    $this->connect();
  }

  public function __destruct() {
    $this->close();
  }

  protected function connect() {
    if (!$this->dbh) {
      try {
        $this->dbh = new PDO('mysql:host=' . $this->server . ';dbname=' . $this->db, $this->user, $this->passwd);
        $this->dbh->query('SET NAMES utf8');
        
        // set the error reporting attribute
        $this->dbh->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
      }
      catch (PDOException $e) {
        throw new Exception('Error connecting to database: '. $e->getMessage() .'.');
      }
    }
  }

  protected function close() {
    $this->dbh = null;
  }

  public function execute($statement, $data = null) {
    // Perpare statement
    $query = $this->dbh->prepare($statement);

    // Execute query and write log
    if ($data != null) {
      $query->execute($data);
    }
    else {
      $query->execute();
    }

    // Log
    return $query;
  }

  public function getLastInsertedId() {
    return $this->dbh->lastInsertId();
  }

  public static function getInstance() {
    if (self::$instance === null) {
      self::$instance = new PDOMysql();
    }
    return self::$instance;
  }
}

?>
