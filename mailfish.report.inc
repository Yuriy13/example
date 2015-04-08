<?php
/**
 * @file
 * Admin settings for the mailfish module.
 */
 
/**
 * Build admin settings for the mailfish module.
 */
function mailfish_report_page() {
  $result_subscribes = db_select('mailfish', 'm')
    ->fields('m')
    ->execute()
    ->fetchAllAssoc('sid');;
  
  $result_nodes = db_select('mailfish_nodes', 'm')
    ->fields('m')
    ->execute()
    ->fetchAll();

  if (!$result_nodes) {
    return '<h3>' . t("Report is empty") . '</h3>';
  }
  
  $header = array(
    t("Node"),
    t("Node Type"),
    t("User"),
    t("Email"),
    t("Date"),
    t("Unsubscribe"),
  );
  $rows = array();
  
  foreach ($result_nodes as $element) {
    $user_fields = user_load($result_subscribes[$element->sid]->uid);
    $node = node_load($element->nid);
  
  $rows[] = array(l($node->title, 'node/' . $node->nid, array('absolute' => TRUE)),
                    $node->type,
                    $user_fields->name,
                    $result_subscribes[$element->sid]->email,
                    format_date($element->created, 'mediun'),
                    l(t("Unsubscribe"), 'admin/mailfish/' . $element->sid . '/node/' . $element->sid . '/delete', array('absolute' => TRUE)),
    );
  }
  
  $output = theme('table', array('header' => $header, 'rows' => $rows));
   
  return $output;
}

/**
 * Build delete form for the mailfish module.
 */
function mailfish_delete_form($form, &$form_state) {
  $sid = $nid = 0;
  if (!($nid = filter_var(arg(2), FILTER_SANITIZE_NUMBER_INT)) || !($sid = filter_var(arg(4), FILTER_SANITIZE_NUMBER_INT))) {
    drupal_set_message(t('Something goes wrong!'), 'error');
    return false;
  }
  
  return confirm_form($form,
    t('Are you sure you want to delete subscription id - %sid for node id - %nid', array('%sid' => $sid, '%nid' => $nid)),
    'admin/reports/mailfish/',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function mailfish_delete_form_submit($form, &$form_state) {  
  $sid = $nid = 0;
  if (!($nid = filter_var(arg(2), FILTER_SANITIZE_NUMBER_INT)) || !($sid = filter_var(arg(4), FILTER_SANITIZE_NUMBER_INT))) {
    drupal_set_message(t('Something goes wrong!'), 'error');
    return false;
  }
  
  $result = db_delete('mailfish_nodes')
    ->condition('sid', $sid, '=')
    ->condition('nid', $nid, '=')
    ->execute();
  
  $result = db_select('mailfish_nodes', 'm')
    ->fields('m')
    ->condition('sid', $sid, '=')
    ->execute()
    ->fetch();
    
  if (!$result) {
    $result = db_delete('mailfish')
      ->condition('sid', $sid, '=')
      ->execute();
      
    $result = db_delete('mailfish_types')
      ->condition('sid', $sid, '=')
      ->execute();
  }
  
  drupal_goto('admin/reports/mailfish/');
}
