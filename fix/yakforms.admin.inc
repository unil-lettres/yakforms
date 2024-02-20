<?php

/**
 * @file
 * Contains Yakforms custom function related to admin menus.
 */

/**
 * Returns the statistics dashboard page.
 *
 * Accesibble at /admin/yakforms/dashboard.
 *
 * @return string
 *   The HTML content for the dashboard.
 */
function get_dashboard_content() {
    global $base_url;
    $content = "<ul>";
    $content .= "<li>";
    // Append otal number of forms.
    $query = db_select('node')->condition('type', 'form1');
    $totalNumberOfForms = $query->execute()->rowCount();
    $content .= "<h3>" . t('Total number of forms : @number', array('@number' => $totalNumberOfForms)) . "</h3>";

    // Append tatistics about the forms created last weeks / months / year.
    $intervals = ["1 week", "1 month", "6 months", "1 year"];
    foreach ($intervals as $interval) {
        $query = "SELECT *
          FROM node
          WHERE type ='form1'
          AND to_timestamp(created) > NOW() - interval'{$interval}'";
        $results = db_query($query, array())->rowCount();
        $content .= "<h4>" . t("Last @interval : @results",
                array(
                    '@interval' => $interval,
                    '@results' => $results,
                )) . "</h4>";
    }
    $content .= "</li>";

    $content .= "<li>";
    // Total number of submissions.
    $query = db_select('webform_submissions');
    $results = $query->execute()->rowCount();
    if ($totalNumberOfForms != 0) {
        $averageSubmissions = floor($results / $totalNumberOfForms * 100) / 100;
    } else {
        $averageSubmissions = 0;
    }
    $content .= "<h3>" . t("Total number of submissions : @number", array('@number' => $results)) . " </h3>";
    $content .= "<h4>" . t("(For an average of @number submissions / form)", array('@number' => $averageSubmissions)) . " </h4>";
    $content .= "</li>";

    $content .= "<li>";
    // Total number of users.
    $query = db_select('users');
    $results = $query->execute();
    $content .= "<h3>" . t("Total number of users : @number", array('@number' => $results->rowCount())) . "</h3>";

    // Users created since a week / month / year.
    $query = "SELECT uid
      FROM users
      WHERE to_timestamp(created) >= NOW() - interval'%s'";

    foreach ($intervals as $interval) {
        $results = db_query(sprintf($query, $interval), array())
            ->rowCount();
        $content .= "<h4>" . t("Registered since @interval : @number",
                array(
                    '@interval' => $interval,
                    '@number' => $results,
                )) . "</h4>";
    }
    $content .= "</li>";

    $content .= "<li>";
    // "Critical forms" (with most submissions)
    $content .= "<h3>" . t("Forms with the most submissions :") . "</h3>";
    $content .= "<ul>";
    $query = "SELECT count(webform_submissions.nid) as NB,
        webform_submissions.nid as NID,
        node.title as TITLE
        from webform_submissions
        inner join node on node.nid = webform_submissions.nid
        group by webform_submissions.nid, node.title
        order by NB DESC
        limit 10";
    $results = db_query($query, array());
    foreach ($results as $result) {
        $content .= "<li>" . $result->title . " (#{$result->nid}) :  {$result->nb} " . t("submissions") . "</li>";
        $content .= "<a href='{$base_url}/node/{$result->nid}'>" . t("Visit") . "</a>";
    }
    $content .= "</ul>";

    // "Critical" users (authors of the most forms)/
    $content .= t('<h3>Users with the most forms</h3>');
    $content .= "<ul>";
    $query = "SELECT users.uid as uid, users.name as name, COUNT(node.nid) as c
      FROM node
      INNER JOIN users ON users.uid = node.uid
      WHERE node.type='form1'
      AND users.uid != 0
      GROUP BY users.uid
      ORDER BY c DESC
      LIMIT 10;";
    $results = db_query($query, array());
    foreach ($results as $result) {
        $content .= "<li> <a href='{$base_url}/user/{$result->uid}'> {$result->name} </a> (#{$result->uid})
      - <a href='{$base_url}/form-search?status=1&nid=&title=&uid={$result->name}&mail='>{$result->c} forms</a> </li>";
    }
    $content .= "</ul>";

    $content .= "<li>";
    $query = "SELECT pg_size_pretty(pg_database_size(current_database()))
    AS dbsize";
    $sizeOfDatabase = db_query($query, array())->fetchAssoc()['dbsize'];
    $content .= "<h3>" . t("Size of database :  @size", array('@size' => $sizeOfDatabase)) . "</h3>";
    $content .= "</li>";
    $content .= "</ul>";

    return $content;
}

/**
 * Implements hook_form().
 */
function yakforms_config_form($form, &$form_state) {
    $form['general-section'] = array(
        '#type' => 'fieldset',
        '#title' => "General information",
        '#collapsible' => TRUE,
    );
    $form['general-section']['info_intertitle'] = array(
        '#type' => 'item',
        '#markup' => "<i>" . t("Note : some of these values can be changed elsewhere on the website, they are simply centralized here.") . "</i>",
    );
    $form['general-section']['site_name'] = array(
        '#type' => 'textfield',
        '#title' => t("Website title"),
        '#default_value' => variable_get('site_name'),
        '#description' => t("The title of this website. It will be displayed in the top header."),
        '#required' => TRUE,
    );
    $form['general-section']['yakforms_general_header_html'] = array(
        '#type' => 'textfield',
        '#title' => t("Website header"),
        '#default_value' => variable_get('yakforms_general_header_html'),
        '#description' => t("The title that will be displayed at the top of your site. You might use the following HTML tags :") . htmlspecialchars("<i><strong><b><p><div><del><h1><h2><h3<h4>"),
        '#required' => TRUE,
    );
    $form['general-section']['site_slogan'] = array(
        '#type' => 'textfield',
        '#title' => t("Website slogan"),
        '#default_value' => variable_get('site_slogan'),
        '#description' => t("Small phrase displayed under the title. Ex : 'Create forms easily'"),
        '#required' => FALSE,
    );
    $form['general-section']['site_mail'] = array(
        '#type' => 'textfield',
        '#title' => t("General contact email"),
        '#default_value' => variable_get('site_mail'),
        '#description' => t("It will be used in the general contact form of the website, and used to warn you of pending updates."),
        '#required' => TRUE,
    );
    $form['general-section']['site_contact_url'] = array(
        '#type' => 'textfield',
        '#title' => t("Link to support"),
        '#default_value' => variable_get('site_contact_url'),
        '#description' => t("Link to a support page for your users. This will be displayed when if your users encounter problems creating forms."),
        '#required' => TRUE,
    );
    $form['limitation-section'] = array(
        '#type' => 'fieldset',
        '#title' => 'Limitations',
        '#collapsible' => TRUE,
    );
    $form['limitation-section']['formsperuser-intertitle'] = array(
        '#type' => 'item',
        '#markup' => "<h4>" . t("Number of forms per user") . "<h4>",
    );
    $form['limitation-section']['yakforms_forms_per_user_limit'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum number of form per user'),
        '#default_value' => variable_get('yakforms_forms_per_user_limit'),
        '#description' => t("0 for no limitation."),
        '#required' => TRUE,
        '#element_validate' => array('element_validate_number'),
    );
    $form['limitation-section']['yakforms_submissions_per_form_limit'] = array(
        '#type' => 'textfield',
        '#title' => t('Maximum number of submissions per form'),
        '#default_value' => variable_get('yakforms_submissions_per_form_limit', 0),
        '#description' => t("0 for no limitation."),
        '#required' => TRUE,
        '#element_validate' => array('element_validate_number'),
    );
    $form['limitation-section']['expiration_intertitle'] = array(
        '#type' => 'item',
        '#markup' => "<h4>" . t("Forms expiration") . "<h4>",
    );
    $form['limitation-section']['yakforms_expiration_period_default_value'] = array(
        '#type' => 'textfield',
        '#title' => t("Expiration period (in weeks)"),
        '#default_value' => variable_get('yakforms_expiration_period_default_value'),
        '#description' => t("When a user creates a form, its expiration date will be set by default at this interval in the futur. Ex : if a user creates a form on 01/01/2020 and the expiration value is set to '5', the form will expire by default on 02/05/2020. Insert '0' if you don't want forms to expire."),
        '#required' => TRUE,
        '#element_validate' => array('element_validate_number'),
    );
    $form['limitation-section']['yakforms_deletion_period_value'] = array(
        '#type' => 'textfield',
        '#title' => t("Deletion period (in weeks)"),
        '#default_value' => variable_get('yakforms_deletion_period_value'),
        '#description' => t("Interval of time between the form expiration and its deletion from the database. Insert '0' if you don't want forms to be deleted."),
        '#required' => TRUE,
        '#element_validate' => array('element_validate_number'),
    );
    $form['limitation-section']['yakforms_export_form_submissions_before_delete'] = array(
        '#type' => 'checkbox',
        '#title' => t("Save form submissions before deleting"),
        '#default_value' => variable_get('yakforms_export_form_submissions_before_delete', FALSE),
        '#description' => t("Check this box if you want all form submissions to be saved before deletion. This can be useful for user support."),
        '#required' => FALSE,
    );
    $form['email-section'] = array(
        '#type' => 'fieldset',
        '#title' => 'Emails',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,

    );
    $form['email-section']['yakforms_mail_user_notification_subject'] = array(
        '#type' => 'textfield',
        '#title' => t("Notification email"),
        '#description' => t("The subject of notification emails. You can use the following tokens : <strong>[node:title]</strong>."),
        '#default_value' => variable_get('yakforms_mail_user_notification_subject'),
        '#required' => TRUE,
    );
    $form['email-section']['yakforms_mail_user_notification_body'] = array(
        '#type' => 'textarea',
        '#title' => t("Notification email"),
        '#description' => t("Here you can change the content of the email that will be sent to the user warning them of expired forms. <i>You can use the following tokens that will be replaced : <strong>[node:url]</strong> (the form URL), <strong>[node:title]</strong> (the form's title), <strong>[yakforms-form:expiration-date]</strong> (the form's expiration date), <strong>[yakforms-form:deletion-date]</strong> (the form's date of deletion).</i>"),
        '#default_value' => variable_get('yakforms_mail_user_notification_body'),
        '#required' => TRUE,
    );
    $form['email-section']['yakforms_mail_from_address'] = array(
        '#type' => 'textfield',
        '#title' => t("Notification 'from' email address"),
        '#description' => t("The default 'from' address set in the email header."),
        '#default_value' => variable_get('yakforms_mail_from_address'),
        '#required' => TRUE,
    );
    $form['defaults-section'] = array(
        '#type' => 'fieldset',
        '#title' => 'Default parameters',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );
    $form['defaults-section']['create-default-pages'] = array(
        '#type' => 'button',
        '#value' => t("Generate default pages"),
        '#submit' => array('admin_menu_create_pages_callback'),
        '#executes_submit_callback' => TRUE,
    );
    $form['defaults-section']['create-default-pages-description'] = array(
        '#type' => 'item',
        '#markup' => "<i>" . t("By clicking this button, you will create default pages for your website : homepage, 404/403 error pages, etc. You can modify them afterwards.") . "</i>",
    );
    $form['defaults-section']['reset-variables'] = array(
        '#type' => 'button',
        '#value' => t("Reset all"),
        '#submit' => array('yakforms_admin_menu_reset_variables_callback'),
        '#executes_submit_callback' => TRUE,
    );
    $form['defaults-section']['reset-variables-description'] = array(
        '#type' => 'item',
        '#markup' => "<i>" . t("Reset all above variables to their initial value.") . "</i>",
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t("Submit"),
        '#submit' => array('system_settings_form_submit'),
    );
    return $form;
}

/**
 * Callback for the "Generate pages" button.
 */
function admin_menu_create_pages_callback() {
    module_load_include('inc', 'yakforms', 'includes/yakforms.pages');
    create_all_pages();
    drupal_set_message(t("The default pages were created."));
}

/**
 * Callback for the "reset variable" button.
 */
function yakforms_admin_menu_reset_variables_callback() {
    module_load_include('install', 'yakforms', 'yakforms');
    yakforms_set_default_variables();
    drupal_set_message(t("All global variables for Yakforms were reset."), 'status', FALSE);
}

/**
 * Inplements hook_form_validate().
 *
 * Check the admin menu's values.
 */
function yakforms_config_form_validate($form, &$form_state) {
    $values = $form_state['values'];
    // Create the backup directory for form submissions if necessary.
    $backup_dir_path = variable_get('file_private_path') . "/yakforms_submissions_backup/";
    $backup_dir_writable = file_prepare_directory($backup_dir_path, FILE_CREATE_DIRECTORY);
    if ($values['yakforms_export_form_submissions_before_delete'] && !$backup_dir_writable) {
        drupal_set_message(
            t("The following directory doesn't exist or is not writable : !path.", array('!path' => $backup_dir_path)),
            'error',
            FALSE
        );
    }
    // Strip disallowed HTML tags.
    $stripped_header = strip_tags($values['yakforms_general_header_html'],
        "<i><strong><b><p><div><del><h1><h2><h3<h4>");

    // Check if disallowed HTML tags were stripped. If so, warn the user.
    if ($stripped_header != $values['yakforms_general_header_html']) {
        form_set_error('general-section][yakforms_general_header_html]', 'Contains disallowed tags.');
    }
    // Make sure the user input is healthy.
    $slogan = $values['site_slogan'];
    $title = $values['site_name'];
    if ($slogan != filter_xss($slogan) || $title != filter_xss($title)) {
        form_set_error('general-section][site_slogan]', 'Some section could be dangerous and is forbidden (contains Javascript code).');
    }
}
