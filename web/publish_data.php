<?php
    ini_set('display_errors', 1);
    ini_set('display_startup_errors', 1);
    error_reporting(E_ALL);

    require_once 'database.php';
    require_once 'env.php';

    header("Access-Control-Allow-Origin: *");
    header("Content-Type: application/json");
    header("Access-Control-Allow-Methods: GET, POST, PUT, DELETE");
    header("Access-Control-Allow-Headers: Content-Type");

    $database = new Database($ENV_HOST, $ENV_DB_NAME, $ENV_USERNAME, $ENV_PASSWORD);

    http_response_code(200);
    /*
    [
        {
            "device": "raspberry_ynov",
            "monitoring": {
                "temperature": 30,
                "humidity": 40
            }
        },
        {
            "device": "device_ynov",
            "data": {
                "temperature": 25,
                "humidity": 50
            },
            "monitoring": {
                "temperature": 30,
                "humidity": 40
            }
        }
    ]
    */

    switch($_SERVER['REQUEST_METHOD']) {
        case 'POST':
            $data = json_decode(file_get_contents('php://input'), true);
            $response = array();
            
            if(!empty($data)) {
                foreach($data as $key => $values) {
                    if(!empty($values['monitoring'])) {
                        foreach($values['monitoring'] as $key2 => $values2) {
                            $insert = $database->db->prepare('INSERT INTO monitoring (device, type, values_data) VALUES (?, ?, ?)');
                            $insert->execute(array($values['device'], $key2, $values2));
                        }
                    }
                    if(!empty($values['data'])) {
                        foreach($values['data'] as $key2 => $values2) {
                            $insert = $database->db->prepare('INSERT INTO values_devices (uuid, device, value_name, value_data) VALUES (?, ?, ?, ?)');
                            $insert->execute(array(uniqid(), $values['device'], $key2, $values2));
                        }
                    }
                    $response = array('error' => 'Added.');

                    $req = $database->db->prepare('SELECT * FROM devices WHERE name = ?');
                    $req->execute(array($values['device']));

                    if($req->rowCount() != 1) {
                        $create = $database->db->prepare('INSERT INTO devices (name, status, ip) VALUES (?, ?, ?)');
                        $create->execute(array($values['device'], 1, (!empty($_SERVER['HTTP_CLIENT_IP'])) ? $_SERVER['HTTP_CLIENT_IP'] : $_SERVER['REMOTE_ADDR']));
                        $response = array('error' => 'Created.');
                    }
                }
            } else {
                $response = array('error' => 'No data provided');
            }

            echo json_encode($response);
            break;
        default:
            echo json_encode(array('error' => 'Method not allowed'));
            break;
    }
    
?>