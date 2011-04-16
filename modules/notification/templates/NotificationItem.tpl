{*
 * $Revision: 847 $
 * If you want to customize this file, do not edit it directly since future upgrades
 * may overwrite it.  Instead, copy it into a new directory called "local" and edit that
 * version.  Gallery will look for that file first and use it if it exists.
 *}

<div class="gbBlock gcBackground1">
    <h2> {g->text text="Item Notification Settings"} </h2>
</div>

<form action="{g->url}" method="post" enctype="application/x-www-form-urlencoded"
 id="notificationItemForm">
<div>
    {g->hiddenFormVars}
    <input type="hidden" name="{g->formVar var="controller"}" 
	value="notification.NotificationItem" />
    <input type="hidden" name="{g->formVar var="form[formName]"}" value="{$form.formName}" />
    <input type="hidden" name="{g->formVar var="itemId"}" value="{$NotificationItem.itemId}"/>
</div>

{if isset($status.saved)}
<div class="gbBlock">
    <h2 class="giSuccess"> {g->text text="Settings saved successfully"} </h2>
</div>
{/if}

{* Methods *}
{capture name=methodOptions}{html_options options=$NotificationMethodOptions}{/capture}
<div class="gbBlock">
    <table class="gbDataTable">
	<tr>
	    <th colspan="4"> {g->text text="Methods"} </th>
	</tr>
    {foreach from=$NotificationMethods item=method name=methods}
	{if $smarty.foreach.methods.iteration % 30 == 1}
	<tr>
	    <th> {g->text text="Name"} </th>
	    <th colspan="3"> {g->text text="Description"} </th>
	</tr>
	{/if}
	<tr>
	    <td>{$method.methodName}</td>
	    <td colspan="3">{$method.methodDescription}</td>
	</tr>
    {/foreach}
	<tr>
	    <td colspan="4"> &nbsp; </td>
	</tr>
{* Events *}
{foreach from=$NotificationEvents item=event name=events}
	{if $smarty.foreach.events.iteration % 30 == 1}
	<tr>
	    <th> {g->text text="Name"} </th>
	    <th> {g->text text="Description"} </th>
	    <th> {g->text text="Method"} </th>
	    <th> {g->text text="Subscribe"} </th>
	</tr>
	{/if}
	<tr>
	    <td rowspan="1"> {$event.eventName} </td>
	    <td>{$event.eventDescription}</td>
	    <td>
	    {if $NotificationMethodOptions|@count == 1}
		<input type="hidden" 
		    name="{g->formVar var="form[events][`$event.eventClass`][methodClass]"}" 
		    value="{$NotificationMethodOptions|@key}">
		{$NotificationMethodOptions|@current}
	    {else}
		<select name="{g->formVar var="form[events][`$event.eventClass`][methodClass]"}">
		{if empty($NotificationSubscriptions[$event.eventClass])}
		    {$smarty.capture.methodOptions}
		{else}
		    {html_options options=$NotificationMethodOptions 
			selected=$NotificationSubscriptions[$event.eventClass]}
		{/if}
		</select>
	    {/if}
	    </td>
	    <td><input type="checkbox" 
		name="{g->formVar var="form[events][`$event.eventClass`][subscribe]"}"
		{if !empty($NotificationSubscriptions[$event.eventClass])} checked="checked"{/if} /></td>
	</tr>
{/foreach}
    </table>
</div>

<div class="gbBlock gcBackground1">
    <input type="submit" class="inputTypeSubmit"
	name="{g->formVar var="form[action][save]"}" value="{g->text text="Save"}" />
    <input type="reset" class="inputTypeSubmit" value="{g->text text="Reset"}"/>
</div>
</form>
