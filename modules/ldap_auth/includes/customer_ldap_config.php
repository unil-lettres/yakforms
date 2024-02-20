<?php
/**
 * @file
 * Contains customer config class.
 */
//include 'includes/utility.php';
/**
 * @file
 * This class represents configuration for customer.
 */
class MiniorangeLdapConfig {

  /**
   * Constructor.
   */
  public function __construct() {

  }

  /**
   * Login using LDAP.
   */
  public function ldapLogin($username, $password) {
    if (!MiniorangeLdapUtility::isCurlInstalled()) {
      return 'CURL_ERROR';
    }
    elseif (!MiniorangeLdapUtility::isExtensionInstalled('mcrypt')) {
      return 'MCRYPT_ERROR';
    }

    $url = MiniorangeLdapConstants::BASE_URL . '/moas/api/ldap/authenticate';
   
    // Calls to encrypt username and password.
    $encryption_key = variable_get('miniorange_ldap_customer_admin_token', '');
   
    $encrypted_username = MiniorangeLdapUtility::encrypt($username, $encryption_key);
    $encrypted_password = MiniorangeLdapUtility::encrypt($password, $encryption_key);
    
    $data = $this->getLoginConfig($encrypted_username, $username, $encrypted_password, 'User Login through LDAP', NULL);
  
    $data_string = json_encode($data);
    $curl = curl_init();

    curl_setopt_array($curl, array(
      CURLOPT_RETURNTRANSFER => 1,
      CURLOPT_URL => $url,
      CURLOPT_POST => 1,
      CURLOPT_POSTFIELDS => $data_string,
      CURLOPT_FOLLOWLOCATION => 1,
      CURLOPT_SSL_VERIFYPEER => TRUE,
      CURLOPT_HTTPHEADER => array(
        'Content-Type: application/json',
        'Content-Length: ' . strlen($data_string),
      ),
    ));
    $response = curl_exec($curl);
    
    if (curl_errno($curl)) {
      $error = array(
        '%method' => 'ldapLogin',
        '%file' => 'customer_ldap_config.php',
        '%error' => curl_error($curl),
      );
      watchdog('miniorange_ldap', "cURL Error at %method of %file: %error", $error);
    }
    curl_close($curl);
    return $response;
  }

  /**
   * Get Login Configuration.
   */
  public function getLoginConfig($encrypted_username, $username, $encrypted_password, $request_type, $is_default) {
    $customer_id = variable_get('miniorange_ldap_customer_id', '');
    $fields = array(
      'customerId' => $customer_id,
      'userName' => $encrypted_username,
      'password' => $encrypted_password,
      'ldapAuditRequest' => array(
        'endUserEmail' => $username,
        'applicationName' => $_SERVER['SERVER_NAME'],
        'appType' => 'Drupal LDAP Login Module',
        'requestType' => $request_type,
      ),
    );

    return $fields;
  }

}
