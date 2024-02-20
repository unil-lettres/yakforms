<?php

drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

include_once 'ldap_auth.module';
include_once 'includes/Utilities.php';
function ldap_auth_ldap_config($form, $form_state)
{
    global $base_url;
    $status='';

    $status=variable_get('miniorange_ldap_config_status','two');

    $form['mo_header_style'] = array('#markup' => '<div class="mo_ldap_table_layout_1">');
    $form['prefix1'] = array('#markup' => '<strong style="color: red">Note: </strong>If you want to test any of our premium feature, you can request a demo by dropping us an email at <a href=\'mailto:drupalsupport@xecurify.com\'>drupalsupport@xecurify.com</a><br><br>');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_ldap_table_layout mo_ldap_container">',
    );
    $form['prefix'] = array('#markup' => '<strong>Note: </strong>You need to find out the values of the below given field from your LDAP administrator</strong><a class="btn btn-primary btn-large btn-right" href="https://plugins.miniorange.com/guide-to-configure-ldap-ad-integration-module-for-drupal" target="_blank">Setup Guide</a>
        <br><br>');

    if(!MiniorangeLdapUtility::isLDAPInstalled()){
        $form['markup_reg_msg'] = array(
            '#markup' => '<div class="mo_ldap_enable_extension_message"><b>The PHP LDAP extension is not enabled.</b><br> Please Enable the PHP LDAP Extension for you server to continue. If you want, you refer to the steps given on the link  <a target="_blank" href="https://faq.miniorange.com/knowledgebase/how-to-enable-php-ldap-extension/" >here</a> to enable the extension for your server.</div><br>',
        );
    }
    if($status=='review_config')
    {
        $form['miniorange_ldap_enable_ldap_markup'] = array(
            '#markup' => "<h1><b>Login With LDAP</b></h1><hr><br>",
        );

       $form['miniorange_ldap_enable_ldap'] = array(
            '#type' => 'checkbox',
            '#title' => t('Enable Login with LDAP '),
            '#default_value' => variable_get('miniorange_ldap_enable_ldap'),
        );

    $form['set_of_radiobuttons']['miniorange_ldap_authentication'] = array(
        '#type' => 'radios',
        '#disabled' => true,
        '#title' => t('Authentication restrictions: <a href="' . $base_url .'/admin/config/people/ldap_auth/license">[Premium]</a>'),
        '#description' => t('Only particular personalities will be able to login by selecting the above option.'),
        '#tree' => TRUE,
        '#default_value' => 0,
        '#options' => array(0 => t(' User can login using both their Drupal or LDAP credentials'), 1 => t(' User can login in Drupal using their LDAP credentials and Drupal admins can also login using their local Drupal credentials'), 2 => t('Users can only login using their LDAP credentials')),
    );

    $form['miniorange_ldap_enable_auto_reg'] = array(
        '#type' => 'checkbox',
        '#disabled' => 'true',
        '#title' => t('Enable Auto Registering users if they do not exist in Drupal <a href="'.$base_url.'/admin/config/people/ldap_auth/license"><b>[Premium]</b></a>'),
        '#default_value' => variable_get('miniorange_ldap_enable_auto_reg', '0'),
        );

    $form['ldap_server'] = array(
        '#markup' => "<br><br>
            <p>You need to find out the values of the below given field from your LDAP administrator</p><br>
            <h1><b>LDAP Connection Information</b></h1><hr><br>",
        );
    $form['miniorange_ldap_server'] = array(
	        '#type' => 'textfield',
	        '#title' => t('LDAP Server'),
            '#id' => 'mo_ldap_server',
            '#required' => TRUE,
	        '#default_value' => variable_get('miniorange_ldap_server'),
	        '#description' => "Specify the host name for the LDAP server eg: ldap://myldapserver.domain:389 , ldap://89.38.192.1:389. When using SSL, the host may have to take the form ldaps://host:636",
            '#attributes' => array('placeholder' => 'ldap://<server-address or IP>:<port>'),
        );
    $form['miniorange_ldap_enable_tls'] = array(
        '#type' => 'checkbox',
        '#id' => 'check',
        '#disabled' => true,
        '#title' => t('Enable TLS (Check this only if your server use TLS Connection) <a href="'.$base_url.'/admin/config/people/ldap_auth/license"><b>[Premium]</b></a>'),
    );
    $form['miniorange_ldap_contact_server_button'] = array(
        '#type' => 'submit',
        '#suffix' => "<br><br>",
        '#limit_validation_errors' => array(),
        '#value' => t('Contact LDAP Server'),
        '#submit' => array('test_ldap_conn'),
    );
    $form['miniorange_ldap_server_account_username'] = array(
	        '#type' => 'textfield',
	        '#title' => t('Bind Account Username:'),
            '#id' => 'test_username',
            '#required' => TRUE,
	        '#default_value' => variable_get('miniorange_ldap_server_account_username'),
	        '#description' => "This service account username will be used to establish the connection. Specify the Service Account Username of the LDAP server in the either way as follows Username@domainname or domainname\Username. or Distinguished Name(DN) format",
            '#attributes' => array('placeholder' => 'CN=service,DC=domain,DC=com'),
    );

    $form['miniorange_ldap_server_account_password'] = array(
	        '#type' => 'password',
	        '#title' => t('Bind Account Password:'),
            '#id' => 'test_password',
            '#required' => FALSE,
            '#default_value' => variable_get('miniorange_ldap_server_account_password'),
            '#attributes' => array(
                    'placeholder' => 'Enter password for Service Account',
                    'value' => variable_get('miniorange_ldap_server_account_password')
            ),
        );


    $form['miniorange_ldap_test_connection_button'] = array(
        '#type' => 'submit',
        '#suffix' => "<br><br>",
        '#value' => t('Test Connection'),
        '#submit' => array('test_connection_ldap'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class'=>array('my-form-class')),
    );

    $form['miniorange_ldap_search_base'] = array(
	        '#type' => 'textfield',
	        '#title' => t('Search Base(s):'),
            '#id' => 'firstname',
            '#required' => TRUE,
            '#description' => 'This is the LDAP Tree under which we will search for the users for authentication. If we are not able to find a user in LDAP it means they are not present in this search base or any of its sub trees. They may be present in some other .
                Provide the distinguished name of the Search Base object. <b>eg. cn=Users,dc=domain,dc=com.
                Multiple Search Bases</b> are supported in the Premium version of the module.',
	        '#default_value' => variable_get('miniorange_ldap_search_base'),
            '#attributes' => array('placeholder' => 'DC=domain, DC=com'),
    );
    $form['miniorange_ldap_username_attribute'] = array(
	        '#type' => 'textfield',
	        '#title' => t('Username/Search Filter:'),
            '#id' => 'firstname',
            '#required' => TRUE,
            '#description' => 'This field is important for two reasons.
            1. While searching for users, this is the attribute that is going to be matched to see if the user exists.
            2. If you want your users to login with their username or firstname.lastname or email - you need to specify those options in this field. e.g. <b>LDAP_ATTRIBUTE</b>. Replace <b><LDAP_ATTRIBUTE></b> with the attribute where your username is stored. Some common attributes are</p>
            <table>
                <tr>
                    <td>common name</td>
                    <td>cn</td>
                    <td></td>
                    <td></td>
                </tr>
                <tr>
                    <td>email</td>
                    <td>mail</td>
                    <td></td>
                    <td></td>
                </tr>
                <tr>
                    <td>logon name</td>
                    <td>samaccountName</td>
                    <td>or</td>
                    <td>userPrincipalName</td>
                </tr>
            </table>
            <p>You can even allow logging in with multiple attributes, separated with  <b>semicolon</b> . e.g. you can allow logging in with username or email. <strong>e.g. cn;mail</strong></p>',
	        '#default_value' => variable_get('miniorange_ldap_username_attribute'),
            '#attributes' => array('placeholder' => 'eg. mail'),
    );
    $form['save_user_mapping'] = array(
        '#markup' => "
                <p>Please make clear that the attributes that we are showing are examples and the actual ones could be different. These should be confirmed with the LDAP Admin.<p>
            <br>",
    );

    $form['back_step_3'] = array(
    '#type' => 'submit',
    '#value' => t('Reset Configurations'),
    '#limit_validation_errors' => array(),
    '#submit' => array('miniorange_ldap_back_2'),
    '#id' => 'button_config',
    );

     $form['save_config_edit'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Test Changes'),
        '#submit' => array('miniorange_ldap_review_changes'),
        '#id' => 'button_config',
        '#attributes' => array('style'=>'margin-left:10%;'),
    );

    }

  if($status=='one')
  {

    $form['miniorange_ldap_enable_ldap_markup'] = array(
        '#markup' => "<h1><b>Login With LDAP</b></h1><hr><br>",
    );

    $form['miniorange_ldap_enable_ldap'] = array(
        '#type' => 'checkbox',
        '#description' => '<b>Note: </b>Enabling LDAP login will protect your login page by your configured LDAP. Please check this only after you have successfully tested your configuration as the default WordPress login will stop working',
        '#title' => t('Enable Login with LDAP '),
        '#default_value' => variable_get('miniorange_ldap_enable_ldap'),
    );

    $form['set_of_radiobuttons']['miniorange_ldap_authentication'] = array(
        '#type' => 'radios',
        '#disabled' => true,
        '#title' => t('Authentication restrictions: <a href="' . $base_url .'/admin/config/people/ldap_auth/license">[Premium]</a>'),
        '#description' => t('Only particular personalities will be able to login by selecting the above option.'),
        '#tree' => TRUE,
        '#default_value' => 0,
      '#options' => array(0 => t(' User can login using both their Drupal or LDAP credentials'), 1 => t(' User can login in Drupal using their LDAP credentials and Drupal admins can also login using their local Drupal credentials'), 2 => t('Users can only login using their LDAP credentials')),
    );

    $form['miniorange_ldap_enable_auto_reg'] = array(
        '#type' => 'checkbox',
        '#disabled' => 'true',
        '#title' => t('Enable Auto Registering users if they do not exist in Drupal <a href="'.$base_url.'/admin/config/people/ldap_auth/license"><b>[Premium]</b></a>'),
        '#default_value' => variable_get('miniorange_ldap_enable_auto_reg', '0'),
    );
    $form['back_step_55'] = array(
        '#type' => 'submit',
        '#id' => 'button_config',
        '#value' => t('BACK'),
        '#submit' => array('miniorange_ldap_back_5'),
    );
    $form['next_step_1'] = array(
        '#type' => 'submit',
        '#id' => 'button_config',
        '#value' => t('Save & Review Configurations'),
        '#submit' => array('miniorange_ldap_next_1'),
        '#attributes' => array('style' => 'margin-left: 10%;'),
    );

    }

    else if($status=='two')
    {
        $form['ldap_server'] = array(
        '#markup' => "
            <b><span style='font-size: 17px;'>LDAP Connection Information</span></b><br><hr><br/>",
        );

        $form['miniorange_ldap_server'] = array(
	        '#type' => 'textfield',
	        '#title' => t('LDAP Server'),
            '#id' => 'mo_ldap_server',
	        '#default_value' => variable_get('miniorange_ldap_server'),
            '#required' => TRUE,
	        '#description' => "Specify the host name for the LDAP server eg: ldap://myldapserver.domain:389 , ldap://89.38.192.1:389. When using SSL, the host may have to take the form ldaps://host:636",
            '#attributes' => array('placeholder' => 'ldap://<server-address or IP>:<port>'),
        );
        $form['miniorange_ldap_contact_server_button'] = array(
            '#type' => 'submit',
            '#suffix' => "<br><br><br>",
            '#value' => t('Contact LDAP Server'),
            '#limit_validation_errors' => array(),
            '#submit' => array('test_ldap_conn'),
        );
        $form['miniorange_ldap_enable_tls'] = array(
            '#type' => 'checkbox',
            '#id' => 'check',
            '#disabled' => true,
            '#title' => t('Enable TLS (Check this only if your server use TLS Connection) <a href="'.$base_url.'/admin/config/people/ldap_auth/license"><b>[Premium]</b></a>'),
        );
        $form['miniorange_ldap_server_account_username'] = array(
	        '#type' => 'textfield',
	        '#title' => t('Service Account Username:'),
            '#id' => 'firstname',
            '#required' => TRUE,
	        '#default_value' => variable_get('miniorange_ldap_server_account_username'),
	        '#description' => "This service account username will be used to establish the connection. Specify the Service Account Username of the LDAP server in the either way as follows Username@domainname or domainname\Username. or Distinguished Name(DN) format",
            '#attributes' => array('placeholder' => 'CN=service,DC=domain,DC=com'),
        );

        $form['miniorange_ldap_server_account_password'] = array(
	        '#type' => 'password',
	        '#title' => t('Service Account Password:'),
            '#id' => 'firstname',
	        '#default_value' => variable_get('miniorange_ldap_server_account_password'),
            '#attributes' => array('placeholder' => 'Enter password for Service Account'),
        );
        $form['miniorange_ldap_test_connection_button'] = array(
            '#type' => 'submit',
            '#value' => t('Test Connection'),
            '#submit' => array('test_connection_ldap'),
        );
        $form['next_step_2'] = array(
            '#type' => 'submit',
            '#id' => 'button_config',
            '#value' => t('NEXT'),
            '#submit' => array('miniorange_ldap_next_2'),
            '#attributes' => array('style' => 'margin-left: 41%;'),
        );
}

else if($status=='three')
{
    $form['miniorange_ldap_search_base'] = array(
    '#type' => 'textfield',
    '#title' => t('Search Base(s):'),
    '#id' => 'firstname',
    '#required' => TRUE,
    '#description' => 'This is the LDAP Tree under which we will search for the users for authentication. If we are not able to find a user in LDAP it means they are not present in this search base or any of its sub trees. They may be present in some other .
        Provide the distinguished name of the Search Base object. <b>eg. cn=Users,dc=domain,dc=com.
        Multiple Search Bases are supported in the Premium version of the module.</b>',
    '#default_value' => variable_get('miniorange_ldap_search_base'),
    '#attributes' => array('placeholder' => 'DC=domain, DC=com'),
        );
    $form['miniorange_ldap_username_attribute'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Username Attribute:'),
        '#id' => 'firstname',
        '#required' => TRUE,
        '#description' => 'This field is important for two reasons.
            1. While searching for users, this is the attribute that is going to be matched to see if the user exists.
            2. If you want your users to login with their username or firstname.lastname or email - you need to specify those options in this field. e.g. <b>LDAP_ATTRIBUTE</b>. Replace <b><LDAP_ATTRIBUTE></b> with the attribute where your username is stored. Some common attributes are</p>
            <table>
                <tr>
                    <td>common name</td>
                    <td>cn</td>
                    <td></td>
                    <td></td>
                </tr>
                <tr>
                    <td>email</td>
                    <td>mail</td>
                    <td></td>
                    <td></td>
                </tr>
                <tr>
                    <td>logon name</td>
                    <td>samaccountName</td>
                    <td>or</td>
                    <td>userPrincipalName</td>
                </tr>
            </table>
            <p>You can even allow logging in with multiple attributes, separated with  <b>semicolon</b> . e.g. you can allow logging in with username or email. <strong>e.g. cn;mail</strong></p>',
        '#default_value' => variable_get('miniorange_ldap_username_attribute'),
        '#attributes' => array('placeholder' => 'eg. mail'),
    );
       $form['back_step_3'] = array(
        '#type' => 'submit',
        '#id' => 'button_config',
        '#value' => t('BACK'),
        '#limit_validation_errors' => array(),
        '#submit' => array('miniorange_ldap_back_3'),
        '#attributes' => array('class'=>array('my-form-class')),
    );
    $form['next_step1'] = array(
            '#type' => 'submit',
            '#id' => 'button_config',
            '#value' => t('NEXT'),
            '#submit' => array('miniorange_ldap_next_to_1'),
            '#attributes' => array('style'=>'margin-left:75%;'),
    );

    }

    MiniorangeLdapUtility::AddSupportButton($form, $form_state);

    return $form;
}
function miniorange_ldap_next_1($form, $form_state)
{
    variable_set('miniorange_ldap_config_status', 'review_config');
    if(!empty($form['miniorange_ldap_enable_ldap']['#value']))
    {$enable_ldap = $form['miniorange_ldap_enable_ldap']['#value'];
     variable_set('miniorange_ldap_enable_ldap',$enable_ldap);
    }
    if(!empty($form['miniorange_ldap_enable_auto_reg']['#value']))
    {
        $auto_reg_users = $form['miniorange_ldap_enable_auto_reg']['#value'];
        variable_set('miniorange_ldap_enable_auto_reg',$auto_reg_users);
    }
    user_cookie_save(array("mo_ldap_test" => true));
    $error = test_mo_config();
    drupal_set_message(t($error));
}
function test_connection_ldap()
{
    $server_account_username = trim(check_plain($_POST['miniorange_ldap_server_account_username']));
    $server_account_password = trim(check_plain($_POST['miniorange_ldap_server_account_password']));

    variable_set('miniorange_ldap_server_account_username', $server_account_username);
    variable_set('miniorange_ldap_server_account_password', $server_account_password);

    user_cookie_save(array("mo_ldap_test" => true));
    $error = test_mo_config();
    drupal_set_message(t($error));
}
function miniorange_ldap_back_1($form,$form_state)
{
    variable_set('miniorange_ldap_config_status', 'one');
}
function miniorange_ldap_back_2($form,$form_state)
{
    variable_del('miniorange_ldap_enable_ldap');
    variable_del('miniorange_ldap_enable_auto_reg');
    variable_del('miniorange_ldap_server');
    variable_del('miniorange_ldap_server_account_username');
    variable_del('miniorange_ldap_server_account_password');
    variable_del('miniorange_ldap_search_base');
    variable_del('miniorange_ldap_username_attribute');
    variable_del('miniorange_ldap_test_username');
    variable_del('miniorange_ldap_test_password');

    variable_set('miniorange_ldap_config_status', 'two');
}

function miniorange_ldap_back_3($form,$form_state)
{
    variable_set('miniorange_ldap_config_status', 'two');
}
function miniorange_ldap_back_5($form,$form_state)
{
    variable_set('miniorange_ldap_config_status', 'three');
}
function miniorange_ldap_back_4($form,$form_state)
{
    variable_set('miniorange_ldap_config_status', 'four');
}

function miniorange_ldap_next_to_1($form, $form_state)
{
    variable_set('miniorange_ldap_config_status', 'one');

    $searchBase = trim($form['miniorange_ldap_search_base']['#value']);
    variable_set('miniorange_ldap_search_base',$searchBase);

    $usernameAttribute = trim($form['miniorange_ldap_username_attribute']['#value']);
    variable_set('miniorange_ldap_username_attribute',$usernameAttribute);
}

function send_support_query(&$form, $form_state)
    {
        $email = trim($form['miniorange_ldap_email_address']['#value']);
        $phone = $form['miniorange_ldap_phone_number']['#value'];
        $query = trim($form['miniorange_ldap_support_query']['#value']);
        MiniorangeLdapUtility::send_query($email, $phone, $query);
    }

function miniorange_ldap_next_2($form, $form_state)
{
    variable_set('miniorange_ldap_config_status', 'three');

    $mo_ldap_server = trim($form['miniorange_ldap_server']['#value']);
    variable_set('miniorange_ldap_server', $mo_ldap_server);

    $server_account_username = trim($form['miniorange_ldap_server_account_username']['#value']);
    variable_set('miniorange_ldap_server_account_username', $server_account_username);
}


function miniorange_ldap_next_3($form, $form_state)
{
    variable_set('miniorange_ldap_config_status', 'four');

    $searchBase = trim($form['miniorange_ldap_search_base']['#value']);
    variable_set('miniorange_ldap_search_base',$searchBase);

    $usernameAttribute = trim($form['miniorange_ldap_username_attribute']['#value']);
    variable_set('miniorange_ldap_username_attribute',$usernameAttribute);
}

function miniorange_ldap_save_changes($form, $form_state)
{
    variable_set('miniorange_ldap_config_status', 'review_config');

    $searchBase = trim($form['miniorange_ldap_search_base']['#value']);
    variable_set('miniorange_ldap_search_base',$searchBase);

    $usernameAttribute = trim($form['miniorange_ldap_username_attribute']['#value']);
    variable_set('miniorange_ldap_username_attribute',$usernameAttribute);

    $testUsername = trim($form['miniorange_ldap_test_username']['#value']);
    variable_set('miniorange_ldap_test_username',$testUsername);

    $testPassword = trim($form['miniorange_ldap_test_password']['#value']);
    variable_set('miniorange_ldap_test_password',$testPassword);
}

function miniorange_ldap_review_changes($form, $form_state)
{
    $enable_ldap = $form['miniorange_ldap_enable_ldap']['#value'];
    variable_set('miniorange_ldap_enable_ldap',$enable_ldap);

    $auto_reg_users = $form['miniorange_ldap_enable_auto_reg']['#value'];
    variable_set('miniorange_ldap_enable_auto_reg',$auto_reg_users);

    $mo_ldap_server = trim($form['miniorange_ldap_server']['#value']);
    variable_set('miniorange_ldap_server', $mo_ldap_server);

    $server_account_username = trim($form['miniorange_ldap_server_account_username']['#value']);
    variable_set('miniorange_ldap_server_account_username', $server_account_username);

    $server_account_password = trim($form['miniorange_ldap_server_account_password']['#value']);
    if($server_account_password != '')
    {
        variable_set('miniorange_ldap_server_account_password', $server_account_password);
    }

    $searchBase = trim($form['miniorange_ldap_search_base']['#value']);
    variable_set('miniorange_ldap_search_base',$searchBase);

    $usernameAttribute = trim($form['miniorange_ldap_username_attribute']['#value']);
    variable_set('miniorange_ldap_username_attribute',$usernameAttribute);
    user_cookie_save(array("mo_ldap_test" => true));
    drupal_set_message("Configuration updated successfully.");
}
global $base_url;
?>
<script>
	function testConfig(){
        var p2Test = "testConfig";
    	var pathparts = location.pathname.split('/');
        if (location.host == 'localhost') {
            var burl = location.origin+'/'+pathparts[1].trim('/')+'/'; // http://localhost/myproject/
        }else{
            var burl = location.origin; // http://demosite.com
        }
	    var testUrl =burl + p2Test;
	    var myWindow = window.open(testUrl, "TEST LDAP Login", "scrollbars=1 width=800, height=600");
    }
    function ping_server(){
        var ping_server_rem_url = "pingServer";
	    var pathparts = location.pathname.split('/');
        if (location.host == 'localhost') {
            var burl = location.origin+'/'+pathparts[1].trim('/')+'/'; // http://localhost/myproject/
        }else{
            var burl = location.origin; // http://demosite.com
        }
	    var ping_url =burl + ping_server_rem_url;
	    var myWindow = window.open(ping_url, "TEST LDAP Connection", "scrollbars=1 width=800, height=600");
    }
</script>
