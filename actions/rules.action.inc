<?php

/**
 * Rules 2 actions are not integrated through Drupal actions, so no need
 * for this hook to do anything.
 */
function views_bulk_operations_rules_action_info() {
  return FALSE;
}

function views_bulk_operations_rules_action($entity, $context) {
  // If there was a config form, there's a rules_element.
  // If not, fallback to the component key.
  if (isset($context['rules_element'])) {
    $element = $context['rules_element'];
  }
  else {
   $element = rules_action('component_' . $context['component_key']);
  }
  $result = $element->execute($entity);
}

function views_bulk_operations_rules_action_form($form, &$form_state, $context) {
  $entity_key = $context['action']['callback arguments']['entity_key'];
  $entity_type = $context['action']['type'];
  $info = entity_get_info($entity_type);

  $set = rules_action_set(array($entity_key => array('type' => $entity_type, 'label' => $info['label'])));
  $action = rules_action('component_' . $context['action']['key'], array($entity_key . ':select' => $entity_key));
  $set->action($action);
  $action->form($form, $form_state);

  // Remove the form element for the "entity" param. It will be passed in manually.
  unset($form['parameter'][$entity_key]);

  return $form;
}

function views_bulk_operations_rules_action_validate($form, $form_state) {
  rules_ui_form_rules_config_validate($form, $form_state);
}

function views_bulk_operations_rules_action_submit($form, $form_state) {
  return array(
    // Pass on the whole-action set for execution.
    'rules_element' => $form_state['rules_element']->root(),
  );
}
