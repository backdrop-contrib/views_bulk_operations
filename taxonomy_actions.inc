<?php

function views_bulk_operations_action_info() {
  return array(
    'taxonomy_action' => array(
      'type' => 'node',
      'description' => t('Modify node\'s taxonomy terms'),
      'configurable' => TRUE,
      'behavior' => array('changes_node_property'),
      'hooks' => array(),
    ),
  );                         
}

function taxonomy_action(&$node, $context) {
  $terms = array();
  foreach ($context['terms'] as $tid) {
    $terms[$tid] = taxonomy_get_term($tid);
  }
  if (!$context['replace']) {
    $terms = array_merge($terms, taxonomy_node_get_terms($node->nid));
  }
  $node->taxonomy = $terms;
}

function taxonomy_action_form($context) {
  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    foreach (taxonomy_get_tree($vid) as $term) {
      $terms[$term->tid] = $vocabulary->name.': '.$term->name;
    }
  }
  $form['terms'] = array(
    '#type' => 'select',
    '#title' => t('Terms'),
    '#options' => $terms,
    '#default_value' => $context['terms'],
    '#multiple' => TRUE,
    '#size' => min(20, count($terms)),
    '#description' => t('Choose the terms you want to associate with the nodes.'),
  );
  $form['replace'] = array(
    '#type' => 'checkbox',
    '#title' => t('Replace existing terms'),
    '#default_value' => FALSE,
    '#description' => t('Check this box to delete existing terms before adding the selected ones. Leave unchecked to just add the new terms over existing ones.'),
  );
  return $form;
}

function taxonomy_action_submit($form, $form_state) {
  return array(
    'replace' => $form_state['values']['replace'], 
    'terms' => $form_state['values']['terms'],
  );
}

function taxonomy_action_validate($form, $form_state) {
  if (!$form_state['values']['replace'] && empty($form_state['values']['terms'])) {
    form_set_error('terms', t('You did not select any term nor did you choose to replace existing terms. Please select one or more terms.'));
  }
}

