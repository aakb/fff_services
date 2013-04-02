<?php

/**
 * @file
 * Representation of a database fact as an object loaded from the Drupal
 * database.
 */
include_once dirname(__FILE__).'/pdo_mysql.inc';

class Fact {

  private $connection = NULL;
  private $properties = array();
  private $loaded = FALSE;

  public function __construct($values = array()) {
    $this->properties = $values + array(
      'guid' => NULL, // Node nid from Drupal
      'title' => NULL,
      'author' => NULL,
      'time' => NULL,
      'content' => NULL,
      'organization' => NULL,
      'inspirations' => array(),
      'sources' => array(),
      'keywords' => array(),
    );

    // Connect to the database and load the fact.
    $this->connection = PDOMysql::getInstance();
    $this->loadFact();
  }

  /**
   * Implementation of setter methode.
   */
  public function __set($key, $val) {
    if (array_key_exists($key, $this->properties)) {
      $this->properties[$key] = $val;
    }
    else {
      throw new Exception('Property "'.$key.'" does not exists in '.basename(__FILE__).' line '.__LINE__);
    }
  }

  /**
   * Implementation of getter methode.
   */
  public function __get($key) {
    if (array_key_exists($key, $this->properties)) {
      return $this->properties[$key];
    }
    else {
      throw new Exception('Property "'.$key.'" does not exists in '.basename(__FILE__).' line '.__LINE__);
    }
  }

  /**
   * Implements __toString() that returns the object formatted as an unordered
   * list.
   */
  public function __toString() {
    $output = '<ul>';
    foreach ($this->properties as $key => $value) {
      $output .= '<li>'.$key.': '.$value.'</li>';
    }
    $output .= '</ul>';
    return $output;
  }

  /**
   * Returns the properties as an array, if the fact have been loaded.
   *
   * @return type
   */
  public function getFact() {
    if (!$this->loaded) {
      throw new Exception('The fact have not been loaded from the database!', -1);
    }
    return $this->properties;
  }

  /**
   * Loads all keywords, which are linked to the current fact.
   */
  private function loadKeywords() {
    $keywords = array();
    $statement = "SELECT terms.name AS keyword
                    FROM node node
              INNER JOIN field_revision_field_fact_tags tags ON node.nid = tags.entity_id
              INNER JOIN taxonomy_term_data terms ON tags.field_fact_tags_tid = terms.tid
                   WHERE node.type = 'fact' AND node.nid = " . $this->guid;
    $query = $this->connection->execute($statement);
    while ($row = $query->fetch(PDO::FETCH_ASSOC)) {
      $keywords[] = $row['keyword'];
    }
    $this->keywords = $keywords;
  }

  /**
   * Loads all source links, which are linked to the current fact.
   */
  private function loadSources() {
    $sources = array();
    $statement = "SELECT sources.field_fact_source_title AS title,
                         sources.field_fact_source_url AS url
                    FROM node node
              INNER JOIN field_revision_field_fact_source sources ON node.nid = sources.entity_id
                   WHERE node.type = 'fact'
                     AND sources.revision_id = node.vid
                     AND node.nid = " . $this->guid;
    $query = $this->connection->execute($statement);
    while ($row = $query->fetch(PDO::FETCH_ASSOC)) {
      $sources[] = $row;
    }
    $this->sources = $sources;
  }

  /**
   * Loads all inspiration links, which are linked to the current fact.
   */
  private function loadInspirations() {
    $inspirations = array();
    $statement = "SELECT inspirations.field_fact_inspiration_title AS title,
                         inspirations.field_fact_inspiration_url AS url
                    FROM node node
              INNER JOIN field_revision_field_fact_inspiration inspirations ON node.nid = inspirations.entity_id
                   WHERE node.type = 'fact'
                     AND inspirations.revision_id = node.vid
                     AND node.nid = " . $this->guid;
    $query = $this->connection->execute($statement);
    while ($row = $query->fetch(PDO::FETCH_ASSOC)) {
      $inspirations[] = $row;
    }
    $this->inspirations = $inspirations;
  }

  /**
   * Load the current fact from the Drupal database or an random fact if the
   * parameter is TRUE
   *
   * @param bool $random
   *   Load a random fact, defaults to FALSE
   */
  private function loadFact($random = FALSE) {
    if (!$this->loaded) {
      // Select nodes where workflow is publised.
      $statement = "SELECT node.title AS title,
                           node.changed AS time,
                           author.field_fact_author_value AS author,
                           content.field_fact_content_value AS content,
                           organization.field_fact_organization_value AS organization
                      FROM node node
                INNER JOIN field_data_field_fact_author author ON node.nid = author.entity_id
                INNER JOIN field_data_field_fact_content content ON node.nid = content.entity_id
                INNER JOIN field_data_field_fact_organization organization ON node.nid = organization.entity_id
                     WHERE node.type = 'fact' AND node.status = '1'";

      if ($random) {
        $statement .= ' ORDER BY rand() LIMIT 1';
      }
      else {
        $statement .= ' AND node.nid = ' . $this->guid;
      }
      $query = $this->connection->execute($statement);

      // Fetch and insert data into this ovject
      $row = $query->fetch(PDO::FETCH_ASSOC);
      if (!empty($row)) {
        foreach ($row as $key => $value) {
          $this->$key = $value;
        }
      }
      else {
        throw new Exception('Fact with guid "'.$this->guid.'" not found in '.basename(__FILE__).' line '.__LINE__);
      }

      // Load extenden information.
      $this->loadKeywords();
      $this->loadSources();
      $this->loadInspirations();

      // Set the flag that the object have been loaded
      $this->loaded = TRUE;
    }
  }

  /**
   * Loads a number of facts based on a offset and count.
   *
   * @param int $offset
   *  Where to start loading facts.
   * @param int $count
   *  The number of facts to load.
   * @return array $facts
   *  An array with facts.
   */
  public static function getFacts($offset, $count) {
    // Connect to the database an extract the next x id's.
    $connection = PDOMysql::getInstance();
    $guids = array();
    $statement = "SELECT node.nid AS guid
                    FROM node node
                   WHERE node.type = 'fact' AND node.status = '1'
                ORDER BY node.changed
                   LIMIT " . $count . " OFFSET " . $offset;
    $query = $connection->execute($statement);
    while ($row = $query->fetch(PDO::FETCH_ASSOC)) {
      $guids[] = $row['guid'];
    }


    // Get facts.
    $facts = array();
    foreach ($guids as $guid) {
      $fact = new Fact(array('guid' => $guid));
      $facts[$guid] = $fact->getFact();
    }

    return $facts;
  }



  /**
   * Finds a random fact GUID in the database.
   *
   * @return string
   *    A random GUID
   */
  public static function getGUID() {
    $connection = PDOMysql::getInstance();

    $statement = "SELECT node.nid AS guid
                    FROM node node
                   WHERE node.type = 'fact' AND node.status = '1'
                ORDER BY rand() LIMIT 1";
    $query = $connection->execute($statement);
    $row = $query->fetch(PDO::FETCH_ASSOC);
    if (!empty($row)) {
      return $row['guid'];
    }
    else {
      throw new Exception('Fact with guid "'.$this->guid.'" not found in '.__FILE__.' line '.__LINE__);
    }
  }
}
