<?php
    class Database {
        private $host;
        private $db_name;
        private $username;
        private $password;
        public $db;

        public function __construct($host, $db_name, $username, $password) {
            $this->host = $host;
            $this->db_name = $db_name;
            $this->username = $username;
            $this->password = $password;

            try {
                $this->db = new PDO('mysql:host=' . $this->host . ';dbname=' . $this->db_name, $this->username, $this->password);
                $this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
            } catch(PDOException $e) {
                echo 'Connection error: ' . $e->getMessage();
            }
        }

    }
?>