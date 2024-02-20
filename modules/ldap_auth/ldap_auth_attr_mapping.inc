<?php
function ldap_auth_attr_mapping($form, $form_state)
{
    global $base_url;
    drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/bootstrap.min.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));
    drupal_add_css( drupal_get_path('module', 'ldap_auth'). '/css/style_settings.css' , array('group' => CSS_DEFAULT, 'every_page' => FALSE));

    $form['mo_header_style'] = array('#markup' => '<div class="mo_ldap_table_layout_1">');

    $form['prefix'] = array('#markup' => '<strong style="color: red">Note: </strong>If you want to test any of our premium feature, you can request a demo by dropping us an email at <a href=\'mailto:drupalsupport@xecurify.com\'>drupalsupport@xecurify.com</a></strong><br><br>');

    $form['mo_top_div'] = array(
        '#markup' => '<div class="mo_ldap_table_layout mo_ldap_container">',
    );
    if(!MiniorangeLdapUtility::isLDAPInstalled()){
        $form['markup_reg_msg'] = array(
            '#markup' => '<div class="mo_ldap_enable_extension_message"><b>The PHP LDAP extension is not enabled.</b><br> Please Enable the PHP LDAP Extension for you server to continue. If you want, you refer to the steps given on the link  <a target="_blank" href="https://faq.miniorange.com/knowledgebase/how-to-enable-php-ldap-extension/" >here</a> to enable the extension for your server.</div><br>',
        );
    }

    $form['markup_top_vt_start'] = array(
        '#markup' => '<b><span style="font-size: 17px;">Attribute Mapping</span></b><br><hr><br/>'
    );

    $form['markup_top_vt_start1'] = array(
        '#markup' => '<p>This feature allows you to map the user attributes coming from your LDAP server to your user attrbiutes in Drupal. In addition to the below mentioned Drupal attrbiutes, you can also map other custom atributes of Drupal in the <a href="'.$base_url.'/admin/config/people/ldap_auth/license">Premium</a> version of the module </p><br><b>Note:</b> Enter the LDAP attribute names for <b>Email, Phone, First Name</b> and <b>Last Name</b> attributes.<br><br>'
    );

    $form['miniorange_ldap_email_attr'] = array(
        '#type' => 'textfield',
        '#title' => t('Email Attribute: '),
        '#default_value' => variable_get('miniorange_ldap_email_attribute') !== null ? variable_get('miniorange_ldap_email_attribute') : 'mail',
        '#attributes' => array('style' => 'width:73%;','placeholder' => 'Enter Email Attribute'),
        '#description' => t('Your LDAP attribute name in which you are getting your users <b>Email</b>.')
    );

    $form['miniorange_ldap_phone_attr'] = array(
        '#type' => 'textfield',
        '#title' => t('Phone: '),
        '#disabled' => TRUE,
        '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Phone Attribute'),
    );

    $form['miniorange_ldap_first_name_attr'] = array(
        '#type' => 'textfield',
        '#title' => t('First Name: '),
        '#disabled' => TRUE,
        '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter First Name Attribute'),
    );

    $form['miniorange_ldap_last_name_attr'] = array(
        '#type' => 'textfield',
        '#title' => t('Last Name: '),
        '#disabled' => TRUE,
        '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Last Name Attribute'),
    );

    $form['miniorange_ldap_attribute_test'] = array(
        '#type' => 'submit',
        '#prefix' => '<br>',
        '#value' => t('Save configurations'),
        '#submit' => array('miniorange_ldap_attribute_mapping'),
        '#id' => 'button_config',
    );

    $form['markup_custom_attr'] = array(
        '#markup' => '<br><br><br><b><span style="font-size: 17px;">Test Attribute Mapping Configuration [<a href="'.$base_url.'/admin/config/people/ldap_auth/license">Premium</a>]</span></b><br><hr><br/>'
    );

    $form['markup_custom_attr1'] = array(
        '#markup' => '<b>Note:</b> Enter LDAP <b>username</b> to test attribute mapping configurations.<br><br>'
    );

    $form['miniorange_ldap_username_attr'] = array(
        '#type' => 'textfield',
        '#title' => t('Username: '),
        '#disabled' => TRUE,
        '#attributes' => array('style' => 'width:73%;background-color: hsla(0,0%,0%,0.08) !important;','placeholder' => 'Enter Username Attribute'),
    );

    $form['miniorange_ldap_attribute_test_config'] = array(
        '#type' => 'submit',
        '#prefix' => '<br>',
        '#value' => t('Test configurations'),
        '#disabled' => TRUE,
        '#id' => 'button_config',
    );

    MiniorangeLdapUtility::AddSupportButton($form, $form_state);

    return $form;
}

function  miniorange_ldap_attribute_mapping($form, $form_state)
{
    $email_attribute = $form['miniorange_ldap_email_attr']['#value'];
    variable_set('miniorange_ldap_email_attribute',$email_attribute);
    drupal_set_message("Email Attribute saved successfully.");
}


function send_support_query(&$form, $form_state)
{
    $email = trim($form['miniorange_ldap_email_address']['#value']);
    $phone = $form['miniorange_ldap_phone_number']['#value'];
    $query = trim($form['miniorange_ldap_support_query']['#value']);
    MiniorangeLdapUtility::send_query($email, $phone, $query);
}
?>
