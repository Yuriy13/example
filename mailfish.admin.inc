<?php
/**
 * @file
 * Admin settings for the mailfish module.
 */
 
/**
 * Build admin settings for the mailfish module.
 */
 
function mailfish_form_admin() {
  $ntypes = node_type_get_types();
  $mtypes = variable_get('mailfish_ntypes', array());
  $form = array();
  
  foreach ($ntypes as $ntype) {
    $form['mailfish_form_admin'][$ntype->type] = array(
      '#type'  => 'checkbox',
      '#title' => drupal_ucfirst($ntype->type),
    );
    if (isset($mtypes[$ntype->type]) && $mtypes[$ntype->type]) {
      $form['mailfish_form_admin'][$ntype->type]['#default_value'] = '1';
    }
    
    $form['mailfish_form_admin']['setting_' . $ntype->type] = array(
      '#type' => 'container',
      '#states' => array(
        // Hide the settings when the cancel notify checkbox is disabled.
        'invisible' => array(
          ':input[name="' . $ntype->type . '"]' => array('checked' => FALSE),
        ),
      ),
      '#attributes' => array(
        'style' => array(
          'padding-left: 25px;',
        ),
      ),
      'submit_by_node_' . $ntype->type => array(
        '#type'  => 'checkbox',
        '#title' => t('Enable to subscribe by node.'),
      ),
    );
    if (isset($mtypes['submit_by_node_' . $ntype->type]) && $mtypes['submit_by_node_' . $ntype->type]) {
      $form['mailfish_form_admin']['setting_' . $ntype->type]['submit_by_node_' . $ntype->type]['#default_value'] = '1';
    }
  }
  $form['#submit'][] = 'mailfish_form_admin_submit';
  
  return system_settings_form($form);
}

/**
 * Form submission handler for mailfish_form().
 *
 * @see mailfish_form_validate()
 */
function mailfish_form_admin_validate($form, &$form_state) {
  
}

/**
 * Form submission handler for mailfish_form().
 *
 * @see mailfish_form_admin_submit()
 */
function mailfish_form_admin_submit($form, &$form_state) {
  variable_set('mailfish_ntypes', $form_state["values"]);
}
