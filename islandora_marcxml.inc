<?php

function islandora_marcxml_ingest_form($form, &$form_state, $object) {
  // Step 1: Build the initial upload form.
  if (empty($form_state['storage']['islandora_marcxml']['mods'])) {
    return array(
      'marcxml_file' => array(
        '#type' => 'file',
        '#title' => t('MARCXML file'),
        '#description' => t('A file containing a single MARCXML record.'),
        '#required' => TRUE,
      ),
      'content_model' => array(
        '#type' => 'select',
        '#title' => t('Content Model'),
        '#description' => t('The content model with which the item will be ingested.'),
        '#options' => islandora_marcxml_get_models($object, 'MODS'),
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Next'),
      ),
    );
  }

  if (empty($form_state['storage']['islandora_marcxml']['form'])) {
    // Step 2: Show list of possible MODS forms.
  }

  if (!empty($form_state['storage']['islandora_marcxml']['form'])) {
    // Step 3: Build an edit form?
    return xml_form_builder_get_form(
      $form_state['storage']['islandora_marcxml']['form'],
      $form_state['storage']['islandora_marcxml']['mods']
    );
  }
}

function islandora_marcxml_ingest_form_validate($form, $form_state) {
  if (empty($form_state['storage']['islandora_marcxml']['mods'])) {
    // Check the file?
  }
  else {
    
  }
}

function islandora_marcxml_ingest_form_submit(&$form, &$form_state) {
  if (empty($form_state['storage']['islandora_marcxml']['mods'])) {
    // Step 1: Transform the uploaded MARCXML.
    $file = file_save_upload('islandora_marcxml');
    
    $module_path = drupal_get_path('module', 'islandora_marcxml');
    $xsl = DOMDocument::load($module_path . '/xsl/MARC21slim2MODS3-4.xsl');

    $xslt = new XSLTProcessor();
    $xslt->importStylesheet($xsl);

    $doc = DOMDocument::load($file->filepath);

    $mods = $xslt->transform($doc);

    $form_state['storage']['islandora_marcxml']['mods'] = $mods->saveXML();
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Step 2: Ingest the object with the edited MODS.
  }
}
