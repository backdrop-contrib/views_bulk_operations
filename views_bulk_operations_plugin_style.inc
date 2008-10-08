<?php
// $Id$


class views_bulk_operations_plugin_style extends views_plugin_style_table {
  function options(&$options) {
    parent::options($options);
    $options['selected_operations'] = array();
    $options['use_batch'] = TRUE;
    $options['display_type'] = 0;
    $this->populate_operations($options);
  }

  function options_form(&$form, &$form_values) {
    parent::options_form($form, $form_values);
    $form['use_batch'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Batch API to process selected rows.'),
      '#default_value' => $this->options['use_batch'],
    );
    $form['display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display operations as'),
      '#default_value' => $this->options['display_type'],
      '#options' => array(
        t('Dropdown selectbox with Submit button'),
        t('Each action as a separate button'),
      ),
    );
    $form['selected_operations'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Selected operations'),
      '#options' => $this->get_operations_options(), 
      '#default_value' => $this->options['selected_operations'],
    );
  }

  function options_validate(&$form, &$form_values) {
  }

  function render() {
    // We build the groups here to pass them to the node_selector function through the form.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    $this->sets = $sets;

    // Append suffix to avoid clashing between multiple VBOs on same page.
    static $form_suffix;
    if (isset($form_suffix)) {
      $form_suffix++;
    }
    else {
      $form_suffix = 1;
    }
    return drupal_get_form('views_bulk_operations_form_' . $form_suffix, $this);
  }

  function get_selected_operations() {
    $selected = array();
    foreach (array_filter($this->options['selected_operations']) as $key => $operation) {
      $selected[$key] = $this->options['all_operations'][$key]['label'];
    }
    return $selected;
  }

  function get_operation_info($key) {
    $operation = $this->options['all_operations'][$key];
    if ($operation['object'] == 'system') { // System actions: morph them into current view type
      $info = _views_bulk_operations_object_info_for_view($this->view);
      $operation['object'] = $info['type'];
    }
    return $operation;
  }

  private function get_operations_options() {
    $options = array();
    $info = _views_bulk_operations_object_info_for_view($this->view);
    if (!$info) return NULL;
    foreach ($this->options['all_operations'] as $key => $operation) {
      if ($operation['object'] == 'system') { // System actions: morph them into current view type
        $operation['object'] = $info['type'];
      }
      if ($operation['object'] == $info['type']) {
        $options[$key] = $operation['label'] .' ('. $operation['callback'] .')'; 
      }
    }
    return $options;
  }

  private function populate_operations(&$options) {
    $operations = array();

    $node_operations = module_invoke_all('node_operations');
    foreach ($node_operations as $operation) {
      if (empty($operation['callback'])) continue;
      $key = md5($operation['callback']);
      $operations[$key] = array(
        'label'              => $operation['label'],
        'callback'           => $operation['callback'],
        'callback arguments' => isset($operation['callback arguments']) ? $operation['callback arguments'] : '',
        'configurable'       => false,
        'type'               => 'node_operation',
        'object'             => 'node',
      );
    }
    
    $user_operations = module_invoke_all('user_operations');
    foreach ($user_operations as $operation) {
      if (empty($operation['callback'])) continue;
      $key = md5($operation['callback']);
      $operations[$key] = array(
        'label'              => $operation['label'],
        'callback'           => $operation['callback'],
        'callback arguments' => isset($operation['callback arguments']) ? $operation['callback arguments'] : '',
        'configurable'       => false,
        'type'               => 'user_operation',
        'object'             => 'user',
      );
    }

    $action_operations = actions_list() + $this->get_custom_actions();
    foreach ($action_operations as $callback => $values) {
      $key = md5($callback);
      $operations[$key] = array(
        'label'              => $values['description'],
        'callback'           => $callback,
        'callback arguments' => isset($values['parameters']) ? $values['parameters'] : array(),
        'configurable'       => $values['configurable'],
        'type'               => 'action',
        'object'             => $values['type'],
      );
    }

    $options['all_operations'] = $operations;
  }

  private function get_custom_actions() {
    $actions = array();
    $result = db_query("SELECT * FROM {actions} WHERE parameters > ''");
    while ($action = db_fetch_object($result)) {
      $actions[$action->aid] = array(
        'description' => $action->description,
        'type' => $action->type,
        'configurable' => FALSE,
        'parameters' => $action->parameters,
      );
    }
    return $actions;
  }
}

